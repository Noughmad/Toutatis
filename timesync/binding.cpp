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


#include "binding.h"
#include "object.h"

#include <QMetaProperty>
#include <QDateTime>

using namespace TimeSync;

Binding::Binding(const QMetaProperty& property, Object* target)
: QObject(target)
, mTarget(target)
, mTimestampPropertyName(property.name() + QLatin1String("_timestamp"))
{
    connect(target, SIGNAL(property.notifySignal().methodSignature()), this, SLOT(propertyValueChanged()));
}

Binding::~Binding()
{

}

void Binding::propertyValueChanged()
{
    mTarget->setProperty(mTimestampPropertyName.toLatin1(), QDateTime::currentMSecsSinceEpoch());
}
