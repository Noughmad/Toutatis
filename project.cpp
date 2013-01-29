#include "project.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDBusConnection>

Project::Project(qlonglong id, QObject* parent)
: QObject(parent)
, mId(id)
{
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Project/" + name(), this);
}

Project::~Project()
{

}

QString Project::name() const
{
    QSqlQuery query;
    query.prepare("SELECT projects FROM projects WHERE _id=:id");
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
    query.prepare("SELECT projects FROM projects WHERE _id=:id");
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
    query.prepare("SELECT name FROM tasks WHIRE project = :id;");
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
}

#include "project.moc"