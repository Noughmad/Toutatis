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

#include "model.h"
#include <QDateTime>

class QDateTime;
class Task;
class Event : public Model
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.toutatis.Event")
    Q_PROPERTY(QString task READ task WRITE setTask NOTIFY taskChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString message READ type WRITE setMessage NOTIFY messageChanged)
    Q_PROPERTY(QDateTime start READ start WRITE setStart NOTIFY timeChanged)
    Q_PROPERTY(QDateTime end READ end WRITE setEnd NOTIFY timeChanged)
    Q_PROPERTY(qlonglong duration READ duration NOTIFY timeChanged)

public:
    explicit Event(Task* parent = 0);
    explicit Event(const QString& id, Task* parent = 0);
    virtual ~Event();

    T_STRING_FIELD(task, Task)
    T_STRING_FIELD(type, Type)
    T_STRING_FIELD(message, Message)
    T_DATE_FIELD(start, Start)
    T_DATE_FIELD(end, End)

    qlonglong duration() const;

private:
    void init();

signals:
    void taskChanged(const QString& task);
    void typeChanged(const QString& type);
    void messageChanged(const QString& message);
    void timeChanged();

#ifdef DBUS_CPP_2_XML
public slots:
    void remove();
signals:
    void removed();
#endif
};

#endif // EVENT_H
