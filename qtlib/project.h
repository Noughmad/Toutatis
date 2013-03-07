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


#ifndef PROJECT_H
#define PROJECT_H

#include "qtatis_export.h"

#include <QList>
#include <QObject>
#include <QMetaType>

class Task;
class ProjectPrivate;

class QTATIS_EXPORT Project : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(bool valid READ isValid)

    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString client READ client WRITE setClient)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible)
    Q_PROPERTY(QList<Task*> tasks READ tasks NOTIFY tasksChanged)
    
public:
    Project(const QString& id, QObject* parent = 0);
    ~Project();
    
    QString id() const;
    bool isValid() const;
    QList<Task*> tasks() const;
    
    QString name() const;
    void setName(const QString& name);
    
    QString client() const;
    void setClient(const QString& client);
    
    bool isVisible() const;
    void setVisible(bool visible);
    
public slots:
    void updateTasks();
    
signals:
    void tasksChanged();
    void nameChanged(const QString& name);
    void clientChanged(const QString& client);
    void visibleChanged(bool visible);
    
private:
    ProjectPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Project);
};

Q_DECLARE_METATYPE(QList<Project*>)

#endif // PROJECT_H
