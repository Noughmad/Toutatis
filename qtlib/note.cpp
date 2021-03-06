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


#include "note.h"
#include "task.h"
#include "global.h"

class NotePrivate
{
public:
    Task* task;
};

Note::Note(const QString& id, QObject* parent)
: com::noughmad::toutatis::Note(Service, "/Note/" + cleanId(id), QDBusConnection::sessionBus(), parent)
, d_ptr(new NotePrivate)
{
    Q_D(Note);
    d->task = getOrCreateModel<Task>(parent, taskId(), this);
    Q_ASSERT(d->task->isValid());
}

Note::~Note()
{
    delete d_ptr;
}

Task* Note::task() const
{
    Q_D(const Note);
    return d->task;
}
