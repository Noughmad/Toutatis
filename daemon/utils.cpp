#include "utils.h"

#include <QStringList>
#include <QVariant>
#include <QSqlQuery>
#include <QJsonObject>
#include <QMetaProperty>
#include <QJsonDocument>
#include <QDebug>

QStringList Utils::stringList(QSqlQuery& query)
{
    QStringList list;
    while (query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}

QStringList Utils::propertyNames(const QMetaObject& meta)
{
    QStringList names;
    for (int i = meta.propertyOffset(); i < meta.propertyCount(); ++i)
    {
        names << meta.property(i).name();
    }
    return names;
}

QJsonArray Utils::serializeType(const QMetaObject& meta, qlonglong timestamp)
{
    QString className = meta.className();
    QStringList properties;
    properties << "_id" << "modified" << "deleted";
    properties << propertyNames(meta);
    
    QSqlQuery query;
    query.prepare(QString("SELECT %1 FROM %2 WHERE modified>:timestamp OR deleted>:timestamp").arg(properties.join(",")).arg(className));
    query.bindValue(":timestamp", timestamp);
    query.exec();
    
    QJsonArray array;
    while (query.next())
    {
        array.append(serializeObject(properties, query));
    }
    
    return array;
}


QJsonObject Utils::serializeObject(const QStringList& properties, const QSqlQuery& query)
{
    QVariantMap map;
    
    for (int i = 0; i < properties.size(); ++i)
    {
        map[properties[i]] = query.value(i);
    }
    
    return QJsonObject::fromVariantMap(map);
}

