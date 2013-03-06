#ifndef UTILS_H
#define UTILS_H

#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QMetaObject>
#include <QStringList>
#include <QVariant>
#include <QSqlQuery>

class Model;
class QJsonObject;
class QSqlQuery;

namespace Utils
{
    QString slug(const QString& text);
    QStringList stringList(QSqlQuery& query);
    QStringList propertyNames(const QMetaObject& meta);
    
    QJsonObject serializeObject(const QStringList& properties, const QSqlQuery& query);
    template <class T> QJsonArray serialize(qlonglong timestamp);
}

template <class T>
QJsonArray Utils::serialize(qlonglong timestamp)
{
    const QMetaObject& meta = T::staticMetaObject;
    QString className = meta.className();
    QStringList properties = QStringList("_id") + propertyNames(meta);
    
    QSqlQuery query;
    query.prepare(QString("SELECT %1 FROM %2 WHERE modified>:timestamp").arg(properties.join(",")).arg(className));
    query.bindValue(":timestamp", timestamp);
    query.exec();
    
    /**
     * TODO: This function has no special handling for deletions
     * Add it. 
     */

    QJsonArray array;
    while (query.next())
    {
        array.append(serializeObject(properties, query));
    }
}

#endif // UTILS_H
