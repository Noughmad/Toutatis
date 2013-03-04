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


#include "object.h"
#include "binding.h"

#include <QUuid>
#include <QRegExp>
#include <QMetaObject>
#include <QMetaProperty>
#include <QStringList>
#include <QDate>
#include <QVector>

using namespace TimeSync;

class TimeSync::ObjectPrivate
{
public:
    QString id;
};

Object::Object(const QString& id, Object* parent)
: QObject(parent)
, d_ptr(new ObjectPrivate)
{
    Q_D(Object);
    d->id = id;
    if (d->id.isEmpty())
    {
        d->id = QUuid::createUuid().toString();
        d->id.remove(d->id.size()-1, 1);
        d->id.remove(0, 1);
        d->id.replace(QRegExp("[^A-Za-z0-9]"), "_");
        setupTimestamps();
    }

    setupConnections();
}

Object::~Object()
{
    delete d_ptr;
}

QString Object::id() const
{
    Q_D(const Object);
    return d->id;
}

void Object::setupTimestamps()
{
    const QMetaObject* mo = metaObject();

    int offset = staticMetaObject.propertyOffset();
    QVector<QByteArray> names;
    for (int i = offset; i < mo->propertyCount(); ++i)
    {
        QMetaProperty property = mo->property(i);
        names << property.name();
    }

    qlonglong stamp = QDateTime::currentMSecsSinceEpoch();
    foreach (const QByteArray& name, names)
    {
        setProperty(name + "_timestamp", stamp);
    }
}

void Object::setupConnections()
{
    const QMetaObject* mo = metaObject();

    int offset = staticMetaObject.propertyOffset();
    QStringList names;
    for (int i = offset; i < mo->propertyCount(); ++i)
    {
        QMetaProperty property = mo->property(i);
        if (property.notifySignal().isValid())
        {
            Binding* binding = new Binding(property, this);
        }
    }
}
