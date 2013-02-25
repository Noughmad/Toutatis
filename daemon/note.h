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

#include "model.h"

class Task;
class Note : public Model
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.toutatis.Note")
    Q_PROPERTY(QString task READ task WRITE setTask NOTIFY taskChanged)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged)
    Q_PROPERTY(QString content READ content WRITE setContent NOTIFY contentChanged)

public:
    explicit Note(Task* parent = 0);
    explicit Note(const QString& id, Task* parent = 0);
    virtual ~Note();

    T_STRING_FIELD(task, Task)
    T_STRING_FIELD(title, Title)
    T_STRING_FIELD(content, Content)

private:
    void init();

signals:
    void taskChanged(const QString& task);
    void titleChanged(const QString& title);
    void contentChanged(const QString& content);

#ifdef DBUS_CPP_2_XML
public slots:
    void remove();
signals:
    void removed();
#endif
};

#endif // NOTE_H
