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
#include "eventadaptor.h"
#include "task.h"

#include <QVariant>

Event::Event(const QString& id, Task* parent): Model(parent)
{
    initialize<Event, EventAdaptor>(id);
    setTaskId(parent->id());
    
    connect (this, &Event::timeChanged, [=]{
        saveField("duration", start().msecsTo(end()));
    });
}

Event::Event(Task* parent): Model(parent)
{
    initialize<Event, EventAdaptor>();
    setTaskId(parent->id());
    
    connect (this, &Event::timeChanged, [=]{
        saveField("duration", start().msecsTo(end()));
    });
}

Event::~Event()
{

}

T_DEF_STRING_FIELD(Event, taskId, TaskId)
T_DEF_STRING_FIELD(Event, type, Type)
T_DEF_STRING_FIELD(Event, message, Message)

T_DEF_DATE_FIELD_X(Event, start, Start, timeChanged)
T_DEF_DATE_FIELD_X(Event, end, End, timeChanged)

qlonglong Event::duration() const
{
    return getField("duration").toLongLong();
}
