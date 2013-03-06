#include "task.h"
#include "taskadaptor.h"
#include "project.h"
#include "toutatis.h"
#include "utils.h"
#include "event.h"
#include "note.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDBusConnection>

Task::Task(const QString& id, Project* parent) : Model(parent)
{
    initialize<Task, TaskAdaptor>(id);
    setProjectId(parent->id());
    
    foreach (const QString& eventId, eventIds())
    {
        Event* e = new Event(eventId, this);
        connect (e, SIGNAL(removed()), SIGNAL(eventIdsChanged()));
    }
    
    foreach (const QString& noteId, noteIds())
    {
        Note* n = new Note(noteId, this);
        connect (n, SIGNAL(removed()), SIGNAL(noteIdsChanged()));
    }
}

Task::Task(Project* parent) : Model(parent)
{
    initialize<Task, TaskAdaptor>();
    setProjectId(parent->id());
    
    foreach (const QString& eventId, eventIds())
    {
        Event* e = new Event(eventId, this);
        connect (e, SIGNAL(removed()), SIGNAL(eventIdsChanged()));
    }
    
    foreach (const QString& noteId, noteIds())
    {
        Note* n = new Note(noteId, this);
        connect (n, SIGNAL(removed()), SIGNAL(noteIdsChanged()));
    }
}

Task::~Task()
{

}

T_DEF_STRING_FIELD(Task, projectId, ProjectId)
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

QString Task::addEvent(const QString& eventType, const QDateTime& start, const QDateTime& end, const QString& message)
{
    Event* e = new Event(this);
    e->setType(eventType);
    e->setStart(start);
    if (end.isValid())
    {
        e->setEnd(end);
    }
    else
    {
        e->setEnd(start);
    }
    e->setMessage(message);

    emit eventIdsChanged();
    return e->id();
}

QStringList Task::eventIds() const
{
    return getList("Event", "taskId");
}

QString Task::addNote(const QString& title, const QString& contents)
{
    Note* n = new Note(this);
    n->setTitle(title);
    n->setContent(contents);

    emit noteIdsChanged();
    return n->id();
}

QStringList Task::noteIds() const
{
    return getList("Note", "taskId");
}

qlonglong Task::duration() const
{
    QSqlQuery query;
    query.prepare("SELECT sum(duration) FROM Event WHERE taskId=:id");
    query.bindValue(":id", id());
    query.exec();
    
    query.next();
    return query.value(0).toLongLong();
}

