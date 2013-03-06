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


#include "event.h"
#include "task.h"
#include "global.h"

class EventPrivate
{
public:
    Task* task;
};

Event::Event(const QString& id, QObject* parent)
: com::noughmad::toutatis::Event(Service, "/Event/" + id, QDBusConnection::sessionBus(), parent)
, d_ptr(new EventPrivate)
{
    Q_D(Event);
    d->task = getOrCreateModel<Task>(parent, taskId(), this);
    Q_ASSERT(d->task->isValid());
}

Event::~Event()
{
    delete d_ptr;
}

Task* Event::task() const
{
    Q_D(const Event);
    return d->task;
}
