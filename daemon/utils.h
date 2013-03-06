#ifndef UTILS_H
#define UTILS_H

#include "model.h"

#include <QString>

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include <QMetaObject>
#include <QStringList>
#include <QVariant>
#include <QSqlQuery>

class Model;
class Model;
class QJsonObject;
class QSqlQuery;

namespace Utils
{
    QString slug(const QString& text);
    QStringList stringList(QSqlQuery& query);
    QStringList propertyNames(const QMetaObject& meta);
    
    template <class T> QJsonArray serialize(qlonglong timestamp);
    QJsonArray serializeType(const QMetaObject& meta, qlonglong timestamp);
    QJsonObject serializeObject(const QStringList& properties, const QSqlQuery& query);
    
    template <class T> void deserialize(const QJsonArray& data, qlonglong timestamp);
    template <class T> T* getOrCreateObject(const QString& id);
}

template <class T>
QJsonArray Utils::serialize(qlonglong timestamp)
{
    return serializeType(T::staticMetaObject, timestamp);
}

template <class T> void Utils::deserialize(const QJsonArray& data, qlonglong timestamp)
{
    foreach (const QJsonValue& value, data)
    {
        if (!value.isObject())
        {
            continue;
        }
        
        QJsonObject obj = value.toObject();
        QString id = obj["id"].toString();
        QString className = obj["class"].toString();
        
        Model* model = getOrCreateObject<T>(id);
        
        Q_ASSERT(model);
        
        if (obj.contains("deleted") && obj["deleted"].toDouble() > timestamp)
        {
            model->remove();
        }
        
        for (auto it = obj.constBegin(); it != obj.constEnd(); ++it)
        {
            QString key = it.key();
            if (key != "id" && key != "class")
            {
                model->setProperty(key.toLatin1(), it.value().toVariant());
            }
        }
    }
}

template <class T> T* Utils::getOrCreateObject(const QString& id)
{
    T* t = Model::findObject<T>(id);
    if (t)
    {
        return t;
    }
    else
    {
        return new T(id);
    }
}


#endif // UTILS_H
