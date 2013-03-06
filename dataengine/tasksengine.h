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


#ifndef TASKSENGINE_H
#define TASKSENGINE_H

#include <Plasma/DataEngine>
#include "toutatis.h"

class TasksEngine : public Plasma::DataEngine
{
    Q_OBJECT

public:
    TasksEngine(QObject* parent, const QVariantList& args);
    
    virtual bool sourceRequestEvent(const QString& source);
    virtual bool updateSourceEvent(const QString& source);
    
private:
    Toutatis mDaemon;
};

#endif // TASKSENGINE_H
