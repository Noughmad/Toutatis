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

#include "project_interface.h"

class ProjectPrivate
{
public:
    com::noughmad::toutatis::Project* interface;
    QList<Task*> tasks;
};

Project::Project(const QString& id, QObject* parent)
: QObject(parent)
, d_ptr(new ProjectPrivate)
{
    Q_D(Project);
    d->interface = new com::noughmad::toutatis::Project(Service, "/Project/" + id, QDBusConnection::sessionBus(), this);
    connect (d->interface, SIGNAL(taskIdsChanged()), SLOT(updateTasks()));
    connect (d->interface, SIGNAL(nameChanged(QString)), SIGNAL(nameChanged(QString)));
    connect (d->interface, SIGNAL(clientChanged(QString)), SIGNAL(clientChanged(QString)));
    connect (d->interface, SIGNAL(visibleChanged(bool)), SIGNAL(visibleChanged(bool)));
    
    qDebug() << id << d->interface->isValid();
    updateTasks();
}

Project::~Project()
{
    delete d_ptr;
}

QString Project::id() const
{
    Q_D(const Project);
    return d->interface->id();
}

bool Project::isValid() const
{
    Q_D(const Project);
    return d->interface->isValid();
}

QString Project::name() const
{
    Q_D(const Project);
    qDebug() << "Reading project name";
    return d->interface->name();
}

void Project::setName(const QString& name)
{
    Q_D(Project);
    d->interface->setName(name);
}

QString Project::client() const
{
    Q_D(const Project);
    return d->interface->client();
}

void Project::setClient(const QString& client)
{
    Q_D(Project);
    d->interface->setClient(client);
}

bool Project::isVisible() const
{
    Q_D(const Project);
    return d->interface->visible();
}

void Project::setVisible(bool visible)
{
    Q_D(Project);
    d->interface->setVisible(visible);
}

QList< Task* > Project::tasks() const
{
    Q_D(const Project);
    return d->tasks;
}

void Project::createTask(const QString& name)
{
    Q_D(Project);
    d->interface->createTask(name);
}

void Project::updateTasks()
{
    Q_D(Project);
    updateModelList(d->tasks, d->interface->taskIds(), this);
    emit tasksChanged();
}

void Project::remove()
{
    Q_D(Project);
    d->interface->remove();
}
