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


#ifndef TASK_H
#define TASK_H

#include "task_interface.h"

class Note;
class Event;
class Project;

class TaskPrivate;

class Task : public com::noughmad::toutatis::Task
{
    Q_OBJECT
    Q_PROPERTY(Project* project READ project)
    Q_PROPERTY(QList<Event*> events READ events NOTIFY eventsChanged)
    Q_PROPERTY(QList<Note*> notes READ notes NOTIFY notesChanged)
    
public:
    explicit Task(const QString& id, QObject* parent);
    virtual ~Task();
    
    Project* project() const;
    QList<Event*> events() const;
    QList<Note*> notes() const;
    
public slots:
    void updateEvents();
    void updateNotes();
    
signals:
    void eventsChanged();
    void notesChanged();
    
private:
    TaskPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Task);

};

#endif // TASK_H
