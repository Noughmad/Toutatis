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


#include "tasksengine.h"
#include "project.h"
#include "task.h"

const char* ToutatisSource = "Toutatis";

TasksEngine::TasksEngine(QObject* parent, const QVariantList& args): DataEngine(parent, args)
{    
    connect (&mDaemon, SIGNAL(projectsChanged()), SLOT(sendProjects()));
    sendProjects();

    foreach (Project* p, mDaemon.projects())
    {
        connect (p, SIGNAL(tasksChanged()), SLOT(tasksChanged()));
        sendTasks(p);
    }
    
    setData(ToutatisSource, "daemon", QVariant::fromValue(&mDaemon));
}

QStringList TasksEngine::sources() const
{
    return QStringList() << ToutatisSource;
}

bool TasksEngine::sourceRequestEvent(const QString& source)
{
    if (source == ToutatisSource)
    {
        sendProjects();
        foreach (Project* p, mDaemon.projects())
        {
            sendTasks(p);
        }
        return true;
    }
    
    return false;
}

void TasksEngine::sendProjects()
{
    qDebug() << "Sending projects";
    QList<QObject*> objects;
    foreach (Project* p, mDaemon.projects())
    {
        objects << p;
    }
    setData(ToutatisSource, "projects", QVariant::fromValue(objects));
}

void TasksEngine::tasksChanged()
{
    Project* project = qobject_cast<Project*>(sender());
    if (project)
    {
        sendTasks(project);
    }
}


void TasksEngine::sendTasks(Project* project)
{
    QList<QObject*> objects;
    foreach (Task* t, project->tasks())
    {
        objects << t;
    }
    qDebug() << QString("Sending %1 tasks for project %2").arg(objects.size()).arg(project->id());
    setData(ToutatisSource, project->id() + "/tasks", QVariant::fromValue(objects));
}

K_EXPORT_PLASMA_DATAENGINE(toutatis, TasksEngine)