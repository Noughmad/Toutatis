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


#include "xmlstorage.h"
#include "object.h"

#include <QDomDocument>
#include <QDomElement>
#include <QDebug>
#include <QFile>
#include <QVariant>
#include <QDateTime>

using namespace TimeSync;

class TimeSync::XmlStoragePrivate
{
public:
    QDomDocument document;
    QString fileName;
    QDomElement findElementById(const QString& tagName, const QString& id);
};

QDomElement XmlStoragePrivate::findElementById(const QString& tagName, const QString& id)
{
    QDomNodeList list = document.elementsByTagName(tagName);
    for (int i = 0; i < list.size(); ++i)
    {
        if (list.at(i).isElement())
        {
            QDomElement element = list.at(i).toElement();
            if (element.attribute("id") == id)
            {
                return element;
            }
        }
    }

    return QDomElement();
}

XmlStorage::XmlStorage(const QString& fileName)
: d_ptr(new XmlStoragePrivate)
{
    Q_D(XmlStorage);
    d->fileName = fileName;

    QFile file(d->fileName);
    file.open(QIODevice::ReadOnly);
    d->document.setContent(&file);
}

XmlStorage::~XmlStorage()
{
    delete d_ptr;
}

QDomDocument XmlStorage::loadHierarchy()
{
    Q_D(XmlStorage);
    return d->document;
}

void XmlStorage::registerType(const QMetaObject& metaObject)
{
    Q_UNUSED(metaObject);
}

void XmlStorage::syncObject(TimeSync::Object* object)
{
    Q_D(XmlStorage);
    QString tag = object->metaObject()->className();
    QString id = object->id();

    QDomElement element = d->findElementById(tag, id);
    if (element.isNull())
    {
        element = d->document.createElement(tag);
        element.setAttribute("id", object->id());
        d->document.documentElement().appendChild(element);
    }

    Q_ASSERT(!element.isNull());

    for (QDomElement property = element.firstChildElement("property"); property != element.lastChildElement("property"); property = property.nextSiblingElement("property"))
    {
        QByteArray propertyName = property.attribute("name").toLatin1();
        qlonglong storageStamp = property.attribute("timestamp").toLongLong();
        qlonglong objectStamp = object->property(propertyName + "_timestamp").toLongLong();

        if (storageStamp > objectStamp)
        {
            object->setProperty(propertyName, property.attribute("value"));
        }
        else
        {
            switch (object->property(propertyName).type())
            {
                case QVariant::String:
                    property.setAttribute("value", object->property(propertyName).toString());
                    break;
                case QVariant::Int:
                case QVariant::UInt:
                case QVariant::LongLong:
                case QVariant::ULongLong:
                    property.setAttribute("value", object->property(propertyName).toLongLong());
                    break;
                case QVariant::Double:
                    property.setAttribute("value", object->property(propertyName).toDouble());
                    break;
                case QVariant::Date:
                case QVariant::DateTime:
                case QVariant::Time:
                    property.setAttribute("value", object->property(propertyName).toDateTime().toMSecsSinceEpoch());
                    break;
                default:
                    qWarning() << "Unknown type:" << propertyName << object->property(propertyName);
                    break;
            }
            property.setAttribute("timestamp", objectStamp);
        }
    }
}
