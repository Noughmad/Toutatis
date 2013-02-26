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


#ifndef EVENT_H
#define EVENT_H

#include "event_interface.h"

class Task;
class EventPrivate;

class Event : public com::noughmad::toutatis::Event
{
    Q_OBJECT
    Q_PROPERTY(Task* task READ task)
    
public:
    Event(const QString& id, QObject* parent);
    virtual ~Event();
    
    Task* task() const;
    
private:
    EventPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Event);
};

#endif // EVENT_H
