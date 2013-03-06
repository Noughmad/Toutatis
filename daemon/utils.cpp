#include "utils.h"
#include "model.h"

#include <QStringList>
#include <QVariant>
#include <QSqlQuery>
#include <QJsonObject>
#include <QMetaProperty>
#include <QJsonDocument>

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

void Utils::deserialize(const QByteArray& data, qlonglong timestamp)
{
    QJsonArray array = QJsonDocument::fromJson(data).array();
    
    foreach (const QJsonValue& value, array)
    {
        if (!value.isObject())
        {
            continue;
        }
        
        QJsonObject obj = value.toObject();
        QString id = obj["id"].toString();
        QString className = obj["class"].toString();
        
        Model* model = Model::findObject(id);
        if (!model)
        {
            // TODO: Create a new model of the appropriate class
        }
        
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
        
        // TODO: Again, handle remote deletions
    }
}
