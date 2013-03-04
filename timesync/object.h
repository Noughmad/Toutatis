/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  <copyright holder> <email>

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


#ifndef TIMESYNC_OBJECT_H
#define TIMESYNC_OBJECT_H

#include <QObject>

namespace TimeSync {

class ObjectPrivate;
class Object : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)

public:
    explicit Object(const QString& id = QString(), Object* parent = 0);
    virtual ~Object();

    QString id() const;

private:
    void setupConnections();

private:
    ObjectPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Object)
    void setupTimestamps();
};

}

#endif // TIMESYNC_OBJECT_H
