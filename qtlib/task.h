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

#include "qtatis_export.h"

#include <QObject>
#include <QMetaType>
#include <QDateTime>

class Note;
class Event;
class Project;

class TaskPrivate;

class QTATIS_EXPORT Task : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(bool valid READ isValid)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QDateTime lastStart READ lastStart NOTIFY activeChanged)
    Q_PROPERTY(qlonglong duration READ duration)
    Q_PROPERTY(Project* project READ project)
    Q_PROPERTY(QList<Event*> events READ events NOTIFY eventsChanged)
    Q_PROPERTY(QList<Note*> notes READ notes NOTIFY notesChanged)
    
public:
    explicit Task(const QString& id, QObject* parent);
    virtual ~Task();
    
    bool isValid() const;
    
    Project* project() const;
    QList<Event*> events() const;
    QList<Note*> notes() const;
    
    QString id() const;
    
    QString name() const;
    void setName(const QString& name);
    
    QString status() const;
    void setStatus(const QString& status);
    
    bool isActive() const;
    void setActive(bool active);
    
    qlonglong duration() const;
    QDateTime lastStart() const;
    
public slots:
    void updateEvents();
    void updateNotes();
    
signals:
    void nameChanged(const QString& name);
    void statusChanged(const QString& status);
    void activeChanged(bool active);

    void eventsChanged();
    void notesChanged();
    
private:
    TaskPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Task);

};

Q_DECLARE_METATYPE(QList<Task*>)

#endif // TASK_H
