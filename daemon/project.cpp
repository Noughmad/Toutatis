#include "project.h"
#include "projectadaptor.h"
#include "task.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDBusConnection>

Project::Project(qlonglong id, QObject* parent)
: QObject(parent)
, mId(id)
{
    new ProjectAdaptor(this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    bool ok = dbus.registerObject("/Toutatis/" + name(), this);

    QSqlQuery tasks;
    tasks.prepare("SELECT _id FROM tasks WHERE project=:id");
    tasks.bindValue(":id", mId);
    tasks.exec();
    while (tasks.next())
    {
        qDebug() << "Creating a task with id " << tasks.value(0);
        new Task(tasks.value(0).toLongLong(), this);
    }
}

Project::~Project()
{

}

QString Project::fullObjectPath() const
{
    return QString("/Projects/%1").arg(mId);
}

QString Project::name() const
{
    QSqlQuery query;
    query.prepare("SELECT name FROM projects WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}

void Project::setName(const QString& name)
{
    QSqlQuery query;
    query.prepare("UPDATE projects SET name=:name WHERE _id=:id");
    query.bindValue(":id", mId);
    query.bindValue(":name", name);
    query.exec();
}

QString Project::client() const
{
    QSqlQuery query;
    query.prepare("SELECT client FROM projects WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}

void Project::setClient(const QString& client)
{
    QSqlQuery query;
    query.prepare("UPDATE projects SET client=:client WHERE _id=:id");
    query.bindValue(":id", mId);
    query.bindValue(":client", client);
    query.exec();
}

qlonglong Project::id() const
{
    return mId;
}

void Project::remove()
{
    QSqlQuery query;
    query.prepare("DELETE projects WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

    deleteLater();
}


QStringList Project::tasks()
{
    QSqlQuery query;
    query.prepare("SELECT name FROM tasks WHERE project = :id;");
    query.bindValue(":id", mId);
    query.exec();

    QStringList list;
    while(query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}

void Project::createTask(const QString& task)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (name, project) VALUES (:name, :id);");
    query.bindValue(":name", task);
    query.bindValue(":id", mId);
    query.exec();

    new Task(query.lastInsertId().toLongLong(), this);
}
