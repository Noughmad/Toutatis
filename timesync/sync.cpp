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


#include "sync.h"
#include "istorage.h"

#include <QDomDocument>

using namespace TimeSync;

void Sync::merge(IStorage* one, IStorage* other)
{
    QDomDocument d1 = one->loadHierarchy();
    QDomDocument d2 = other->loadHierarchy();

    merge(d1, d2);

    one->saveHierarchy(d1);
    other->saveHierarchy(d2);
}

void Sync::merge(QDomDocument& one, const QDomDocument& other)
{
    for(QDomElement o1 = one.firstChildElement(); o1 != one.lastChildElement(); o1 = o1.nextSiblingElement())
    {
        if (o1.attribute("id").isEmpty())
        {
            continue;
        }
        QString tag = o1.tagName();
        for(QDomElement o2 = other.firstChildElement(tag); o2 != other.lastChildElement(tag); o2 = o2.nextSiblingElement(tag))
        {
            if (o2.attribute("id") == o1.attribute("id"))
            {
                mergeObject(o1, o2);
                break;
            }
        }
    }
}

QDomElement Sync::mergeObject(const QDomElement& one, const QDomElement& other)
{
    for(QDomElement p1 = one.firstChildElement("property"); p1 != one.lastChildElement("property"); p1 = p1.nextSiblingElement("property"))
    {
        for(QDomElement p2 = other.firstChildElement("property"); p2 != other.lastChildElement("property"); p2 = p2.nextSiblingElement("property"))
        {
            if (p1.attribute("name") == p2.attribute("name"))
            {
                mergeProperty(p1, p2);
                break;
            }
        }
    }
}

void Sync::mergeProperty(QDomElement& one, const QDomElement& other)
{
    qlonglong t1 = one.attribute("timestamp").toLongLong();
    qlonglong t2 = other.attribute("timestamp").toLongLong();

    if (t1 < t2)
    {
        one.setAttribute("value", other.attribute("value"));
        one.setAttribute("timestamp", other.attribute("timestamp"));
    }
}
