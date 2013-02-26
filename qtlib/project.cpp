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


#include "project.h"
#include "task.h"
#include "global.h"

class ProjectPrivate
{
public:
    QList<Task*> tasks;
};

Project::Project(const QString& id, QObject* parent)
: com::noughmad::toutatis::Project(Service, "Project/" + id, QDBusConnection::sessionBus())
, d_ptr(new ProjectPrivate)
{
    connect (this, SIGNAL(taskIdsChanged()), SLOT(updateTasks()));
}

Project::~Project()
{
    delete d_ptr;
}

QList< Task* > Project::tasks() const
{
    Q_D(const Project);
    return d->tasks;
}

void Project::updateTasks()
{
    Q_D(Project);
    updateModelList(d->tasks, taskIds(), this);
    emit tasksChanged();
}

