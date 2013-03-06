#include "utils.h"
#include "model.h"

#include <QStringList>
#include <QVariant>
#include <QSqlQuery>
#include <QJsonObject>
#include <QMetaProperty>

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

QJsonObject Utils::serializeObject(const QStringList& properties, const QSqlQuery& query)
{
    QVariantMap map;
    
    for (int i = 0; i < properties.size(); ++i)
    {
        map[properties[i]] = query.value(i);
    }
    
    return QJsonObject::fromVariantMap(map);
}