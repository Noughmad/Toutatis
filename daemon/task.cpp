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

void Task::addEvent(const QString& eventType, qlonglong start, qlonglong end, const QString& title, const QString& message)
{
    QSqlQuery query;
    query.prepare("INSERT INTO events (task, type, start, end, duration, title, message) VALUES (:task, :type, :start, :end, :duration, :message);");
    query.bindValue(":task", mId);
    query.bindValue(":type", eventType);
    query.bindValue(":start", start);
    query.bindValue(":end", end);
    query.bindValue(":duration", end - start);
    query.bindValue(":message", message);
    query.exec();

    emit eventsChanged();
}

QVector< qlonglong > Task::events() const
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM events WHERE task=:task;");
    query.bindValue(":task", mId);
    query.exec();

    QVector<qlonglong> list;
    while (query.next())
    {
        list << query.value(0).toLongLong();
    }

    return list;
}

qlonglong Task::addNote(const QString& title, const QString& contents)
{
    QSqlQuery query;
    query.prepare("INSERT INTO notes (task, title, contents) VALUES (:task, :title, :contents);");
    query.bindValue(":task", mId);
    query.bindValue(":title", title);
    query.bindValue(":contents", contents);
    query.exec();

    emit notesChanged();
    return query.lastInsertId().toLongLong();
}

QStringList Task::notes() const
{
    QSqlQuery query;
    query.prepare("SELECT text FROM notes WHERE task=:task");
    query.bindValue(":task", mId);
    query.exec();

    QStringList list;
    while (query.next())
    {
        list << query.value(0).toString();
    }

    return list;
}

void Task::removeNote(const QString& title)
{

}

qlonglong Task::id() const
{
    return mId;
}

qlonglong Task::duration() const
{
    // TODO;
    return 0;
}

