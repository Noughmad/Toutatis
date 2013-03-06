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

#include "project_interface.h"
#include "qtatis_export.h"

#include <QList>

class Task;
class ProjectPrivate;

class QTATIS_EXPORT Project : public com::noughmad::toutatis::Project
{
    Q_OBJECT
    Q_PROPERTY(QList<Task*> tasks READ tasks NOTIFY tasksChanged)
    
public:
    Project(const QString& id, QObject* parent = 0);
    ~Project();
    
    QList<Task*> tasks() const;
    
public slots:
    void updateTasks();
    
signals:
    void tasksChanged();
    
private:
    ProjectPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Project);
};

Q_DECLARE_METATYPE(QList<Project*>)

#endif // PROJECT_H
