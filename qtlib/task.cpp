/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  Miha Cancula <email>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


#include "task.h"
#include "global.h"
#include "project.h"
#include "event.h"
#include "note.h"

#include "task_interface.h"

class TaskPrivate
{
public:
    Project* project;
    QList<Event*> events;
    QList<Note*> notes;
    com::noughmad::toutatis::Task* interface;
};

Task::Task(const QString& id, QObject* parent)
: QObject(parent)
, d_ptr(new TaskPrivate)
{
    Q_D(Task);
    d->interface = new com::noughmad::toutatis::Task(Service, "/Task/" + id, QDBusConnection::sessionBus(), this);
    
    qDebug() << "Created a task" << id << d->interface->path();
    
    d->project = getOrCreateModel<Project>(parent, d->interface->projectId(), this);
    Q_ASSERT(d->project->isValid());
    
    connect (d->interface, SIGNAL(eventIdsChanged()), SLOT(updateEvents()));
    connect (d->interface, SIGNAL(noteIdsChanged()), SLOT(updateNotes()));
    
    connect (d->interface, SIGNAL(nameChanged(QString)), SIGNAL(nameChanged(QString)));
    connect (d->interface, SIGNAL(statusChanged(QString)), SIGNAL(statusChanged(QString)));
    connect (d->interface, SIGNAL(activeChanged(bool)), SIGNAL(activeChanged(bool)));
    
    updateEvents();
    updateNotes();
}

Task::~Task()
{
    delete d_ptr;
}

bool Task::isValid() const
{
    Q_D(const Task);
    return d->interface->isValid();
}

QString Task::id() const
{
    Q_D(const Task);
    return d->interface->id();
}

QString Task::name() const
{
    Q_D(const Task);
    return d->interface->name();
}

void Task::setName(const QString& name)
{
    Q_D(Task);
    d->interface->setName(name);
}

QString Task::status() const
{
    Q_D(const Task);
    return d->interface->status();
}

void Task::setStatus(const QString& status)
{
    Q_D(Task);
    d->interface->setStatus(status);
}

qlonglong Task::duration() const
{
    Q_D(const Task);
    return d->interface->duration();
}

QList< Event* > Task::events() const
{
    Q_D(const Task);
    return d->events;
}

bool Task::isActive() const
{
    Q_D(const Task);
    return d->interface->active();
}

void Task::setActive(bool active)
{
    Q_D(Task);
    d->interface->setActive(active);
}

QDateTime Task::lastStart() const
{
    Q_D(const Task);
    return d->interface->lastStart();
}

void Task::updateEvents()
{
    Q_D(Task);
    updateModelList(d->events, d->interface->eventIds(), this);
    emit eventsChanged();
}

QList< Note* > Task::notes() const
{
    Q_D(const Task);
    return d->notes;
}

void Task::updateNotes()
{
    Q_D(Task);
    updateModelList(d->notes, d->interface->noteIds(), this);
    emit notesChanged();
}

Project* Task::project() const
{
    Q_D(const Task);
    return d->project;
}
