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


#ifndef NOTE_H
#define NOTE_H

#include "note_interface.h"

class Task;
class NotePrivate;

class Note : public com::noughmad::toutatis::Note
{
    Q_OBJECT
    Q_PROPERTY(Task* task READ task)
    
public:
    Note(const QString& id, QObject* parent);
    virtual ~Note();
    
    Task* task() const;
    
private:
    NotePrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Note);
};

#endif // NOTE_H
