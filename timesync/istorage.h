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


#ifndef TIMESYNC_ISTORAGE_H
#define TIMESYNC_ISTORAGE_H

class QDomDocument;
class QString;
class QMetaObject;

namespace TimeSync {
class Object;
class IStorage
{
public:
    virtual ~IStorage();

    // TODO: Maybe save and load should by combined into a single method that synchronizes the storage and the object
    virtual bool saveObject(Object* object) = 0;
    virtual Object* loadObject(const QString& type, const QString& id) = 0;
    virtual QDomDocument loadHierarchy() = 0;

    template <class T> void registerType();

protected:
    virtual void registerType(const QMetaObject& metaObject) = 0;
};

}

template <class T> void TimeSync::IStorage::registerType()
{
    registerType(T::staticMetaObject);
}


#endif // TIMESYNC_ISTORAGE_H
