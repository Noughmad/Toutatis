#include "toutatis.h"
#include "toutatisadaptor.h"
#include "project.h"
#include "task.h"

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
        new Project(projects.value(0).toLongLong(), this);
    }

    QSqlQuery currentTask;
    currentTask.exec("SELECT _id FROM tasks WHERE active=true;");
    if (currentTask.next())
    {
        mCurrentTask = currentTask.value(0).toLongLong();
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

QVector< qlonglong > Toutatis::projects() const
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM projects WHERE visible = 1;");
    query.exec();

    QVector<qlonglong> list;
    while(query.next())
    {
        list << query.value(0).toLongLong();
    }
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

    qlonglong id = query.lastInsertId().toLongLong();
    new Project(id, this);

    emit projectsChanged();
    return id;
}

qlonglong Toutatis::currentTask() const
{
    return mCurrentTask;
}

void Toutatis::startTracking(qlonglong task)
{
    stopTracking();

    QSqlQuery query;
    query.prepare("UPDATE tasks SET active=true, lastStart=:start "
    "WHERE _id=:id;");
    query.bindValue(":id", task);
    query.bindValue(":start", QDateTime::currentMSecsSinceEpoch());
    query.exec();

    if (query.numRowsAffected() > 0)
    {
        mCurrentTask = task;
        emit currentTaskChanged(task);
    }
}

void Toutatis::startTracking(const QString& project, const QString& task, bool create)
{
    qlonglong id = findTask(project, task);

    if (id)
    {
        startTracking(id);
    }
    else if (create)
    {
        qlonglong projectId = findProject(project);
        if (!projectId)
        {
            projectId = createProject(project);
        }

        Project p(projectId);
        id = p.findTask(task);
        if (!id)
        {
            id = p.createTask(task);
        }

        startTracking(id);
    }
}

bool Toutatis::isTracking() const
{
    return mCurrentTask > 0;
}

void Toutatis::stopTracking()
{
    if (!mCurrentTask)
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

    emit currentTaskChanged(0);
}

qlonglong Toutatis::findProject(const QString& name)
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM projects WHERE name=:name;");
    query.bindValue(":name", name);
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
