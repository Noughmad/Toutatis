#include "toutatis.h"

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
}

Toutatis::~Toutatis()
{
    mDatabase.close();
}

void Toutatis::createTables()
{
    QSqlQuery query;
    query.exec("CREATE TABLE project "
        "(_id INTEGER PRIMARY KEY, "
        "name TEXT, "
        "client TEXT, "
        "visible INTEGER DEFAULT 1);");

    query.exec("CREATE TABLE task "
        "(_id INTEGER PRIMARY KEY, "
        "project INTEGER REFERENCES project(_id), "
        "name TEXT, "
        "active INTEGER DEFAULT 0, "
        "lastStart INTEGER DEFAULT -1, "
        "status INTEGER DEFAULT 1);");

    query.exec("CREATE TABLE event "
        "(_id INTEGER PRIMARY KEY, "
        "task INTEGER REFERENCES task(_id), "
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
    return list;
}

void Toutatis::createProject(const QString& name, const QString& client)
{
    QSqlQuery query;
    query.prepare("INSERT INTO projects (name, client, visible) VALUES (:name, :client, :visible);");
    query.bindValue(":name", name);
    query.bindValue(":client", client);
    query.bindValue(":visible", 1);
    query.exec();

    emit projectChanged(name);
}

QStringList Toutatis::tasks(const QString& project)
{
    QSqlQuery query;
    query.prepare("SELECT name FROM tasks WHIRE project = :project;");
    query.bindValue(":project", project);

    QStringList list;
    while(query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}

void Toutatis::createTask(const QString& project, const QString& task)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (name, project) VALUES (:name, :project);");
    query.bindValue(":name", task);
    query.bindValue(":project", project);
    query.exec();

    emit taskChanged(project, task);
}

void Toutatis::renameTask(const QString& project, const QString& task, const QString& name)
{

}

void Toutatis::removeTask(const QString& project, const QString& task)
{

}

void Toutatis::currentProjectAndTask(QString& project, QString& task)
{

}

void Toutatis::startTask(const QString& project, const QString& task)
{
    QSqlQuery query;
    query.prepare("UPDATE tasks SET active=true, lastStart=:start WHERE project=:project AND name=:name;");
    query.bindValue(":name", task);
    query.bindValue(":project", project);
    query.bindValue(":start", QDateTime::currentMSecsSinceEpoch());
    query.exec();

    emit currentTaskChanged(project, task);
}

bool Toutatis::isTracking()
{
    return !mCurrentProject.isEmpty() && !mCurrentTask.isEmpty();
}

void Toutatis::stop()
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

void Toutatis::output()
{
}

#include "toutatis.moc"
