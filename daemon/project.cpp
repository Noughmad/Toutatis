#include "project.h"
#include "projectadaptor.h"
#include "task.h"
#include "utils.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDBusConnection>

Project::Project(const QString& id, QObject* parent)
: QObject(parent)
, mId(id)
{
    new ProjectAdaptor(this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Project/" + id, this);

    QSqlQuery tasks;
    tasks.prepare("SELECT _id FROM tasks WHERE project=:id");
    tasks.bindValue(":id", mId);
    tasks.exec();
    while (tasks.next())
    {
        new Task(tasks.value(0).toString(), this);
    }
}

Project::~Project()
{

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

QString Project::id() const
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


QStringList Project::tasks() const
{
    QSqlQuery query;
    query.prepare("SELECT name FROM tasks WHERE project = :id;");
    query.bindValue(":id", mId);
    query.exec();

    return Utils::stringList(query);
}

QString Project::createTask(const QString& task)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (name, project) VALUES (:name, :id);");
    query.bindValue(":name", task);
    query.bindValue(":id", mId);
    query.exec();

    QString id = query.lastInsertId().toString();
    new Task(id, this);

    emit tasksChanged();
    return id;
}

QString Project::findTask(const QString& task)
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM tasks WHERE project=:project AND name=:name");
    query.bindValue(":project", mId);
    query.bindValue(":name", task);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}
