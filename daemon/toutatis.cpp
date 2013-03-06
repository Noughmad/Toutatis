#include "toutatis.h"
#include "toutatisadaptor.h"
#include "project.h"
#include "task.h"
#include "utils.h"
#include "note.h"
#include "event.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include <QSqlError>
#include <QDir>

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

Toutatis::Toutatis(QObject* parent) : QObject(parent)
{
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");

    QDir dir(QDir::homePath() + "/.local/share/toutatis");

    if (!dir.exists())
    {
        qDebug() << dir.absolutePath();
        dir.mkpath(".");
    }
    mDatabase.setDatabaseName(dir.absoluteFilePath("database.db"));

    bool ok = mDatabase.open();
    if (!ok)
    {
        qDebug() << mDatabase.lastError();
    }

    if (mDatabase.tables().isEmpty())
    {
        qDebug() << "Existing database not found, creating tables";
        createTables();
    }
    else
    {
        qDebug() << "Found existing database in " << mDatabase.databaseName();
    }

    Q_ASSERT(mDatabase.tables().size() == 4);

    new ToutatisAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Toutatis", this);
    dbus.registerService("com.noughmad.Toutatis");

    foreach (const QString& projectId, projectIds())
    {
        Project* p = new Project(projectId, this);
        connect (p, SIGNAL(removed()), SIGNAL(projectIdsChanged()));
    }

    QSqlQuery currentTask;
    currentTask.exec("SELECT _id FROM tasks WHERE active=1;");
    if (currentTask.next())
    {
        mCurrentTask = currentTask.value(0).toString();
    }
    emit currentTaskChanged(mCurrentTask);
}

Toutatis::~Toutatis()
{
    stopTracking();
    mDatabase.close();
}

void Toutatis::createTables()
{
    Model::createTable<Project>();
    Model::createTable<Task>();
    Model::createTable<Event>();
    Model::createTable<Note>();
    
    QSqlQuery query;
    query.exec("CREATE TABLE Sync ("
        "_id INTEGER PRIMARY KEY, "
        "time INTEGER, "
        "destination TEXT);");
}

QStringList Toutatis::projectIds() const
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM Project WHERE deleted IS NULL");
    query.exec();

    return Utils::stringList(query);
}

QString Toutatis::createProject(const QString& name, const QString& client)
{
    Project* project = new Project(this);
    project->setName(name);
    project->setClient(client);

    emit projectIdsChanged();
    return project->id();
}

QString Toutatis::currentTask() const
{
    return mCurrentTask;
}

void Toutatis::startTracking(const QString& id)
{
    stopTracking();

    QSqlQuery query;
    query.prepare("UPDATE Task SET active=1, lastStart=:start "
    "WHERE _id=:id;");
    query.bindValue(":id", id);
    query.bindValue(":start", QDateTime::currentDateTime());
    query.exec();

    if (query.numRowsAffected() > 0)
    {
        mCurrentTask = id;
        emit currentTaskChanged(id);
    }
}

void Toutatis::startTracking(const QString& project, const QString& task, bool create)
{
    QString id = findTask(project, task);

    if (!id.isEmpty())
    {
        startTracking(id);
    }
    else if (create)
    {
        QString projectId = findProject(project);
        if (projectId.isEmpty())
        {
            projectId = createProject(project);
        }

        Project* p = Model::findObject<Project>(projectId);
        id = p->findTask(task);
        if (id.isEmpty())
        {
            id = p->createTask(task);
        }

        startTracking(id);
    }
}

bool Toutatis::isTracking() const
{
    return !mCurrentTask.isEmpty();
}

void Toutatis::stopTracking()
{
    if (!isTracking())
    {
        return;
    }

    QDateTime start;
    QSqlQuery select;
    select.exec("SELECT lastStart FROM Task WHERE active=1;");

    if (select.next())
    {
        start = select.value(0).toDateTime();
    }

    Task* task = Model::findObject<Task>(mCurrentTask);
    Q_ASSERT(task);
    task->addEvent("TimeTracking", start, QDateTime::currentDateTime(), QDateTime::currentDateTime().toString());

    QSqlQuery query;
    query.prepare("UPDATE Task SET active=0, lastStart=0 WHERE active=1;");
    query.exec();

    mCurrentTask = QString();
    emit currentTaskChanged(QString());
}

QString Toutatis::findProject(const QString& name)
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM Project WHERE name=:name;");
    query.bindValue(":name", name);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
    {
        return QString();
    }
}

QString Toutatis::findTask(const QString& project, const QString& task)
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM Task WHERE name=:task AND "
        "project IN (SELECT _id FROM Project WHERE name=:project);");
    query.bindValue(":project", project);
    query.bindValue(":task", task);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }
    else
    {
        return QString();
    }
}

void Toutatis::synchronize(const QUrl& destination)
{
    QSqlQuery lastQuery;
    lastQuery.prepare("SELECT time FROM Sync WHERE destination=:url");
    lastQuery.bindValue(":url", destination);
    lastQuery.exec();
    
    qlonglong lastSync = 0;
    if (lastQuery.next())
    {
        lastSync = lastQuery.value(0).toLongLong();
    }
    
    QJsonObject object;
    object["Project"] = Utils::serialize<Project>(lastSync);
    object["Task"] = Utils::serialize<Task>(lastSync);
    object["Event"] = Utils::serialize<Event>(lastSync);
    object["Note"] = Utils::serialize<Note>(lastSync);
    
    QJsonDocument doc(object);
    
    QNetworkRequest request;
    QUrl url(destination);
    url.setPath("/api/sync");
    request.setUrl(url);
    
    qlonglong timestamp = QDateTime::currentMSecsSinceEpoch();
    
    QNetworkAccessManager manager;
    QNetworkReply* reply = manager.post(request, doc.toJson());

    connect (reply, &QNetworkReply::finished, [=] {
        QJsonObject object = QJsonDocument::fromJson(reply->readAll()).object();
        Utils::deserialize<Project>(object["Project"].toArray(), lastSync);
        Utils::deserialize<Task>(object["Task"].toArray(), lastSync);
        Utils::deserialize<Event>(object["Event"].toArray(), lastSync);
        Utils::deserialize<Note>(object["Note"].toArray(), lastSync);
        
        QSqlQuery sync;
        sync.prepare("INSERT INTO Sync (time, destination) VALUES (:timestamp, :url)");
        sync.bindValue(":timestamp", timestamp);
        sync.bindValue(":url", destination);
        sync.exec();
    });
}
