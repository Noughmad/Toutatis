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

TasksEngine::TasksEngine(QObject* parent, const QVariantList& args): DataEngine(parent, args)
{
    setMinimumPollingInterval(200);
    
    /**
     * TODO: Toutatis used signals for everything, so we should not need polling at all here. 
     */
}

bool TasksEngine::sourceRequestEvent(const QString& source)
{
    return updateSourceEvent(source);
}

bool TasksEngine::updateSourceEvent(const QString& source)
{
    if (source == "Projects")
    {
        QStringList projects;
        QObjectList objects;
        foreach (Project* p, mDaemon.projects())
        {
            projects << p->name();
            objects << p;
        }
        setData(source, "ids", mDaemon.projectIds());
        setData(source, "names", projects);
        setData(source, "projects", QVariant::fromValue(objects));
        return true;
    }
    else if (source.startsWith("Project/"))
    {
        QString id = source.mid(8);
        foreach (Project* p, mDaemon.projects())
        {
            if (p->id() == id)
            {
                setData(source, "Tasks", QVariant::fromValue(p->tasks()));
                return true;
            }
        }
    }
    return false;
}

K_EXPORT_PLASMA_DATAENGINE(toutatis, TasksEngine)