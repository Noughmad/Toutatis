#include "task.h"
#include "taskadaptor.h"
#include "project.h"
#include "toutatis.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDBusConnection>

Task::Task(qlonglong id, Project* parent)
: QObject(parent)
, mId(id)
{
    new TaskAdaptor(this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Task/" + QString::number(id), this);
}

Task::~Task()
{

}

QString Task::name() const
{
    QSqlQuery query;
    query.prepare("SELECT name FROM tasks WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}

void Task::setName(const QString& name)
{
    QSqlQuery query;
    query.prepare("UPDATE tasks SET name=:name WHERE _id=:id");
    query.bindValue(":id", mId);
    query.bindValue(":name", name);
    query.exec();
}

void Task::remove()
{
    QSqlQuery query;
    query.prepare("DELETE tasks WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

    deleteLater();
}

QString Task::status() const
{
    QSqlQuery query;
    query.prepare("SELECT status FROM tasks WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}

void Task::setStatus(const QString& status)
{
    QSqlQuery query;
    query.prepare("UPDATE tasks SET name=:name WHERE _id=:id");
    query.bindValue(":id", mId);
    query.bindValue(":name", status);
    query.exec();
}

QDateTime Task::lastStart() const
{
    QSqlQuery query;
    query.prepare("SELECT status FROM tasks WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toDateTime();
    }

    return QDateTime();
}

void Task::setLastStart(const QDateTime& start)
{
    QSqlQuery query;
    query.prepare("UPDATE tasks SET lastStart=:start WHERE _id=:id");
    query.bindValue(":id", mId);
    query.bindValue(":start", start);
    query.exec();
}

bool Task::isActive() const
{
    QSqlQuery query;
    query.prepare("SELECT status FROM tasks WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

    if (query.next())
    {
        return query.value(0).toBool();
    }

    return false;
}

void Task::setActive(bool active)
{
    if (active)
    {
        Toutatis* main = qobject_cast<Toutatis*>(parent()->parent());
        Q_ASSERT(main);

        if (main->isTracking())
        {
            main->stopTracking();
        }
    }

    QSqlQuery query;
    query.prepare("UPDATE tasks SET lastStart=:start active=:active WHERE _id=:id");
    query.bindValue(":id", mId);
    query.bindValue(":active", active);
    query.bindValue(":start", active ? QDateTime::currentDateTime() : QDateTime());
    query.exec();
}

void Task::start()
{
    setActive(true);
}

void Task::stop()
{
    setActive(false);
}
