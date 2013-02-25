#include "task.h"
#include "taskadaptor.h"
#include "project.h"
#include "toutatis.h"
#include "utils.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDBusConnection>

Task::Task(const QString& id, Project* parent) : Model("tasks", id, parent)
{
    setProject(parent->id());
    init();
}

Task::Task(Project* parent) : Model("tasks", parent)
{
    setProject(parent->id());
    init();
}

Task::~Task()
{

}

void Task::init()
{
    new TaskAdaptor(this);
    QDBusConnection dbus = QDBusConnection::sessionBus();
    dbus.registerObject("/Task/" + id(), this);

    foreach (const QString& eventId, events())
    {
        // new Event(eventId, this);
    }

    foreach (const QString& noteId, notes())
    {
        // new Note(noteId, this);
    }
}

T_DEF_STRING_FIELD(Task, project, Project)
T_DEF_STRING_FIELD(Task, name, Name)
T_DEF_STRING_FIELD(Task, status, Status)

QDateTime Task::lastStart() const
{
    return getField("lastStart").toDateTime();
}

void Task::setLastStart(const QDateTime& start)
{
    saveField("lastStart", start);
}

bool Task::isActive() const
{
    return getField("active").toBool();
}

void Task::setActive(bool active)
{
    Toutatis* main = qobject_cast<Toutatis*>(parent()->parent());
    Q_ASSERT(main);

    if (active != isActive())
    {
        main->stopTracking();
    }

    if (active)
    {
        main->startTracking(id());
    }

    emit activeChanged(active);
}

void Task::start()
{
    setActive(true);
}

void Task::stop()
{
    setActive(false);
}

QString Task::addEvent(const QString& eventType, qlonglong start, qlonglong end, const QString& title, const QString& message)
{
    /*
    QSqlQuery query;
    QString id = QUuid::createUuid().toString();
    query.prepare("INSERT INTO events (_id, task, type, start, end, duration, title, message) VALUES (:id, :task, :type, :start, :end, :duration, :message);");
    query.bindValue(":id", id);
    query.bindValue(":task", id());
    query.bindValue(":type", eventType);
    query.bindValue(":start", start);
    query.bindValue(":end", end);
    query.bindValue(":duration", end - start);
    query.bindValue(":message", message);
    query.exec();
    */

    emit eventsChanged();
    return QString();
}

QStringList Task::events() const
{
    return getList("events", "task");
}

QString Task::addNote(const QString& title, const QString& contents)
{
    /*
    QSqlQuery query;
    QString id = QUuid::createUuid().toString();
    query.prepare("INSERT INTO notes (_id, task, title, contents) VALUES (:id, :task, :title, :contents);");
    query.bindValue(":id", id);
    query.bindValue(":task", mId);
    query.bindValue(":title", title);
    query.bindValue(":contents", contents);
    query.exec();

    emit notesChanged();
    return id;
    */
    return QString();
}

QStringList Task::notes() const
{
    return getList("notes", "task");
}

qlonglong Task::duration() const
{
    // TODO;
    return 0;
}

