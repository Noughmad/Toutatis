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

class TaskPrivate
{
public:
    Project* project;
    QList<Event*> events;
    QList<Note*> notes;
};

Task::Task(const QString& id, QObject* parent)
: com::noughmad::toutatis::Task(Service, "Task/ " + id, QDBusConnection::sessionBus(), parent)
, d_ptr(new TaskPrivate)
{
    Q_D(Task);
    d->project = qobject_cast<Project*>(parent);
    Q_ASSERT(d->project);
    
    connect (this, SIGNAL(eventIdsChanged()), SLOT(updateEvents()));
    connect (this, SIGNAL(noteIdsChanged()), SLOT(updateNotes()));
    
}

Task::~Task()
{
    delete d_ptr;
}

QList< Event* > Task::events() const
{
    Q_D(const Task);
    return d->events;
}

void Task::updateEvents()
{
    Q_D(Task);
    updateModelList(d->events, eventIds(), this);
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
    updateModelList(d->notes, noteIds(), this);
    emit notesChanged();
}

Project* Task::project() const
{
    Q_D(const Task);
    return d->project;
}
