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
    /*
    QSqlQuery query;
    query.exec("CREATE TABLE projects "
        "(_id TEXT PRIMARY KEY, "
        "created INTEGER, "
        "modified INTEGER, "
        "name TEXT, "
        "client TEXT, "
        "visible INTEGER DEFAULT 1,);");

    query.exec("CREATE TABLE tasks "
        "(_id TEXT PRIMARY KEY, "
        "created INTEGER, "
        "modified INTEGER, "
        "project INTEGER REFERENCES projects(_id), "
        "name TEXT, "
        "active INTEGER DEFAULT 0, "
        "lastStart INTEGER DEFAULT -1, "
        "status INTEGER DEFAULT 1);");

    query.exec("CREATE TABLE events "
        "(_id TEXT PRIMARY KEY, "
        "created INTEGER, "
        "modified INTEGER, "
        "task INTEGER REFERENCES tasks(_id), "
        "type TEXT, "
        "start INTEGER, "
        "end INTEGER, "
        "message TEXT);");

    query.exec("CREATE TABLE notes "
        "(_id TEXT PRIMARY KEY, "
        "created INTEGER, "
        "modified INTEGER, "
        "task INTEGER REFERENCES tasks(_id), "
        "title TEXT, "
        "content TEXT);");
        */
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
