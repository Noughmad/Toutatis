#include "toutatis.h"
#include "toutatisadaptor.h"
#include "project.h"

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
        qDebug() << "Creating a project with id " << projects.value(0);
        new Project(projects.value(0).toLongLong(), this);
    }
}

Toutatis::~Toutatis()
{
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
}

QStringList Toutatis::projects() const
{
    QSqlQuery query;
    query.prepare("SELECT name FROM projects WHERE visible = 1;");
    query.exec();

    QStringList list;
    while(query.next())
    {
        list << query.value(0).toString();
    }
    qDebug() << "Returning projects: " << list;
    return list;
}

qlonglong Toutatis::createProject(const QString& name, const QString& client)
{
    QSqlQuery query;
    query.prepare("INSERT INTO projects (name, client, visible) VALUES (:name, :client, :visible);");
    query.bindValue(":name", name);
    query.bindValue(":client", client);
    query.bindValue(":visible", 1);
    query.exec();

    emit projectChanged(name);
    return query.lastInsertId().toLongLong();
}

QString Toutatis::currentProjectAndTask(QString& task)
{
    task = mCurrentTask;
    return mCurrentProject;
}

void Toutatis::startTask(const QString& project, const QString& task, bool create)
{
    qDebug() << "Starting task" << project << "/" << task << create;
    /*
     * First try to update any tasks with specified project and name
     */
    QSqlQuery query;
    query.prepare("UPDATE tasks SET active=true, lastStart=:start "
            "WHERE project IN (SELECT _id FROM projects WHERE name=:project) "
            "AND name=:name;");
    query.bindValue(":name", task);
    query.bindValue(":project", project);
    query.bindValue(":start", QDateTime::currentMSecsSinceEpoch());
    query.exec();

    /*
     * TODO: query.numRowsAffected() always returns -1
     * Instead, we should first try to find the task,
     * and then create or start it
     */

    qlonglong taskId = findTask(project, task);
    if (taskId > 0)
    {
        QSqlQuery query;
        query.prepare("UPDATE tasks SET active=true, lastStart=:start "
        "WHERE _id=:id;");
        query.bindValue(":start", QDateTime::currentMSecsSinceEpoch());
        query.bindValue(":id", taskId);
        query.exec();
    }
    else if (create)
    {
        qDebug() << "Task not found, creating";
        QSqlQuery projectQuery;
        projectQuery.prepare("SELECT _id FROM projects WHERE name=:name");
        projectQuery.bindValue(":name", project);
        projectQuery.exec();

        qlonglong projectId;
        if (projectQuery.next())
        {
            projectId = projectQuery.value(0).toLongLong();
        }
        else
        {
            projectId = createProject(project);
        }

        foreach (Project* p, findChildren<Project*>())
        {
            if (p->id() == projectId)
            {
                if (!p->tasks().contains(task))
                {
                    p->createTask(task);
                }
                break;
            }
        }

        startTask(project, task, false);
    }

    emit currentTaskChanged(project, task);
}

bool Toutatis::isTracking()
{
    return !mCurrentProject.isEmpty() && !mCurrentTask.isEmpty();
}

void Toutatis::stopTracking()
{
    if (isTracking())
    {
        stopTask(mCurrentProject, mCurrentTask);
    }
}

void Toutatis::stopTask(const QString& project, const QString& task)
{
    qint64 start;
    QSqlQuery taskQuery;
    taskQuery.exec("SELECT lastStart FROM tasks WHERE active=true;");
    while (taskQuery.next())
    {
        start = taskQuery.value(0).toLongLong();
    }

    addEvent(project, task, "TimeTracking", start, QDateTime::currentMSecsSinceEpoch());

    QSqlQuery query;
    query.prepare("UPDATE tasks SET active=false, lastStart=:start WHERE project=:project AND name=:name;");
    query.bindValue(":name", task);
    query.bindValue(":project", project);
    query.bindValue(":start", QDateTime());
    query.exec();

    emit currentTaskChanged(QString(), QString());
}

void Toutatis::addNote(const QString& project, const QString& task, const QString& note)
{

}

void Toutatis::addEvent(const QString& project, const QString& task, const QString& eventType, qint64 start, qint64 end, const QString& message)
{
    QSqlQuery query;
    query.prepare("INSERT INTO events (project, task, type, start, end, duration, message) VALUES (:project, :task, :type, :start, :end, :duration, :message);");
    query.bindValue(":project", project);
    query.bindValue(":task", task);
    query.bindValue(":start", start);
    query.bindValue(":end", end);
    query.bindValue(":duration", end - start);
    query.bindValue(":message", message);
    query.exec();
}

qlonglong Toutatis::findTask(const QString& project, const QString& task)
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM tasks WHERE name=:task AND "
        "project IN (SELECT _id FROM projects WHERE name=:project);");
    query.bindValue(":project", project);
    query.bindValue(":task", task);
    query.exec();

    if (query.next())
    {
        return query.value(0).toLongLong();
    }
    else
    {
        return 0;
    }
}
