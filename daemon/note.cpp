/*
 *  <one line to give the program's name and a brief idea of what it does.>
 *  Copyright (C) 2013  Miha Cancula <email>
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "note.h"
#include "noteadaptor.h"
#include "task.h"

#include <QVariant>

Note::Note(Task* parent): Model(parent)
{
    initialize<Note, NoteAdaptor>();
    setTaskId(parent->id());
}

Note::Note(const QString& id, Task* parent): Model(parent)
{
    initialize<Note, NoteAdaptor>(id);
    setTaskId(parent->id());
}

Note::~Note()
{

}

T_DEF_STRING_FIELD(Note, taskId, TaskId)
T_DEF_STRING_FIELD(Note, title, Title)
T_DEF_STRING_FIELD(Note, content, Content)