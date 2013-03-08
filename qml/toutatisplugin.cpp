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


#include "toutatisplugin.h"

#include "toutatis.h"
#include "project.h"
#include "task.h"
#include "note.h"
#include "event.h"

#include <QtDeclarative/QtDeclarative>

void ToutatisPlugin::registerTypes(const char* uri)
{
    const QString reason = "Only for DataEngine";
    qmlRegisterType<Toutatis>(uri, 1, 0, "Toutatis");
    qmlRegisterUncreatableType<Project>(uri, 1, 0, "Project", "Project " + reason);
    qmlRegisterUncreatableType<Task>(uri, 1, 0, "Task", reason);
    qmlRegisterUncreatableType<Event>(uri, 1, 0, "Event", reason);
    qmlRegisterUncreatableType<Note>(uri, 1, 0, "Note", reason);
        
    qRegisterMetaType<QList<Project*> >("QList<Project*>");
    qRegisterMetaType<QList<Task*> >("QList<Task*>");
}

Q_EXPORT_PLUGIN2(qmltoutatisplugin, ToutatisPlugin);