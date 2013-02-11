#include "toutatis.h"
#include "toutatisadaptor.h"
#include "project.h"
#include "task.h"
#include "utils.h"

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

    Q_ASSERT(mDatabase.tables().size() == 3);

    new ToutatisAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Toutatis", this);
    dbus.registerService("com.noughmad.Toutatis");

    QSqlQuery projects;
    projects.exec("SELECT _id FROM projects");
    while (projects.next())
    {
        new Project(projects.value(0).toString(), this);
    }

    QSqlQuery currentTask;
    currentTask.exec("SELECT _id FROM tasks WHERE active=true;");
    if (currentTask.next())
    {
        mCurrentTask = currentTask.value(0).toString();
    }
}

Toutatis::~Toutatis()
{
    stopTracking();
    mDatabase.close();
}

void Toutatis::createTables()
{
    QSqlQuery query;
    query.exec("CREATE TABLE projects "
        "(_id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "client TEXT, "
        "visible INTEGER DEFAULT 1);");

    query.exec("CREATE TABLE tasks "
        "(_id INTEGER PRIMARY KEY, "
        "project INTEGER REFERENCES projects(_id), "
        "name TEXT, "
        "active INTEGER DEFAULT 0, "
        "lastStart INTEGER DEFAULT -1, "
        "status INTEGER DEFAULT 1);");

    query.exec("CREATE TABLE events "
        "(_id INTEGER PRIMARY KEY, "
        "task INTEGER REFERENCES tasks(_id), "
        "start INTEGER, "
        "end INTEGER, "
        "duration INTEGER, "
        "message TEXT);");

    query.exec("CREATE TABLE notes "
        "(_id INTEGER PRIMARY KEY, "
        "task INTEGER REFERENCES tasks(_id), "
        "title TEXT, "
        "content TEXT);");
}

QStringList Toutatis::projects() const
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM projects WHERE visible = 1;");
    query.exec();

    return Utils::stringList(query);
}

QString Toutatis::createProject(const QString& name, const QString& client)
{
    QSqlQuery query;
    query.prepare("INSERT INTO projects (name, client, visible) VALUES (:name, :client, :visible);");
    query.bindValue(":name", name);
    query.bindValue(":client", client);
    query.bindValue(":visible", 1);
    query.exec();

    QString id = query.lastInsertId().toString();
    new Project(id, this);

    emit projectsChanged();
    return id;
}

QString Toutatis::currentTask() const
{
    return mCurrentTask;
}

void Toutatis::startTracking(const QString& id)
{
    stopTracking();

    QSqlQuery query;
    query.prepare("UPDATE tasks SET active=true, lastStart=:start "
    "WHERE _id=:id;");
    query.bindValue(":id", id);
    query.bindValue(":start", QDateTime::currentMSecsSinceEpoch());
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

        Project p(projectId);
        id = p.findTask(task);
        if (id.isEmpty())
        {
            id = p.createTask(task);
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

    qlonglong start;
    QSqlQuery select;
    select.exec("SELECT lastStart FROM tasks WHERE active=true;");

    if (select.next())
    {
        start = select.value(0).toLongLong();
    }

    Task task(mCurrentTask);
    task.addEvent("TimeTracking", start, QDateTime::currentMSecsSinceEpoch(), QDateTime::currentDateTime().toString());

    QSqlQuery query;
    query.prepare("UPDATE tasks SET active=false, lastStart=0 WHERE active=true;");
    query.exec();

    emit currentTaskChanged(QString());
}

QString Toutatis::findProject(const QString& name)
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM projects WHERE name=:name;");
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
    query.prepare("SELECT _id FROM tasks WHERE name=:task AND "
        "project IN (SELECT _id FROM projects WHERE name=:project);");
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
