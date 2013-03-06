/*
    <one line to give the program's name and a brief idea of what it does.>
    Copyright (C) 2013  Miha Cancula <email>

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


#include "model.h"

#include <QUuid>
#include <QVariant>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QMetaProperty>
#include <QDebug>

class ObjectStore
{
public:
    QHash<QString, Model*> objects;
};

Q_GLOBAL_STATIC(ObjectStore, store)

Model::Model(QObject* parent)
: QObject(parent)
{
}

Model::~Model()
{

}

void Model::remove()
{
    QSqlQuery query;
    saveField("deleted", QDateTime::currentMSecsSinceEpoch());

    emit removed();
    deleteLater();
}

QString Model::id() const
{
    return mId;
}

QVariant Model::getField(const QString& field) const
{
    QSqlQuery query;
    query.prepare("SELECT " + field + " FROM " + mTableName + " WHERE _id=:id");
    query.bindValue(":id", mId);

    query.exec();
    if (query.next())
    {
        return query.value(0);
    }

    return QVariant();
}

void Model::saveField(const QString& field, const QVariant& value)
{
    QSqlQuery query;
    query.prepare("UPDATE " + mTableName + " SET " + field + "=:value, modified=:timestamp WHERE _id=:id");
    query.bindValue(":value", value);
    query.bindValue(":timestamp", QDateTime::currentMSecsSinceEpoch());
    query.bindValue(":id", mId);
    query.exec();
}

QStringList Model::getList(const QString& table, const QString& key) const
{
    QSqlQuery query;
    query.prepare(QString("SELECT _id FROM %1 WHERE %2=:id AND deleted IS NULL").arg(table).arg(key));
    query.bindValue(":id", mId);
    query.exec();

    QStringList list;
    while (query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}

void Model::registerObject(Model* object)
{
    Q_ASSERT(!store()->objects.contains(object->id()));
    Q_ASSERT(!store()->objects.values().contains(object));

    store()->objects.insert(object->mId, object);
}

Model* Model::findObject(const QString& id)
{
    return store()->objects.value(id, 0);
}

QHash< QString, Model* > Model::allObjects()
{
    return store()->objects;
}

void Model::createTable(const QMetaObject& meta)
{
    QString sql = QString("CREATE TABLE %1 (_id TEXT PRIMARY KEY, created INTEGER, modified INTEGER, deleted INTEGER").arg(meta.className());
    for (int i = meta.propertyOffset(); i < meta.propertyCount(); ++i)
    {
        QMetaProperty p = meta.property(i);
        QString sqlType;
        switch (p.type())
        {
            case QVariant::String:
            case QVariant::Char:
                sqlType = "TEXT";
                break;
            case QVariant::Int:
            case QVariant::LongLong:
            case QVariant::UInt:
            case QVariant::ULongLong:
            case QVariant::DateTime:
            case QVariant::Bool:
                sqlType = "INTEGER";
                break;
            case QVariant::Double:
                sqlType = "REAL";
                break;
                
            case QVariant::StringList:
                // This is probably a list of child ids
                continue;
                
            default:
                qWarning() << "Unsupported field type" << p.type();
                continue;
        }
        sql += QString(", %1 %2").arg(p.name()).arg(sqlType);
    }
    sql += ");";

    QSqlQuery query;
    query.exec(sql);
}

void Model::create()
{
    QSqlQuery query;
    query.prepare(QString("INSERT INTO %1 (_id, created, modified) VALUES (:id, :timestamp, :timestamp)").arg(mTableName));
    query.bindValue(":id", mId);
    query.bindValue(":timestamp", QDateTime::currentMSecsSinceEpoch());
    
    bool ok = query.exec();
    Q_ASSERT(ok);
}

void Model::setupId(const QString& id)
{
    mId = id;
    mTableName = metaObject()->className();

    if (mId.isEmpty())
    {
        mId = QUuid::createUuid().toString();
        mId.remove(mId.size()-1, 1);
        mId.remove(0, 1);
        mId.replace(QRegExp("[^A-Za-z0-9]"), "_");
     
        create();
    }
    else
    {
        QSqlQuery query;
        query.prepare(QString("SELECT COUNT(*) FROM %1 WHERE _id=:id").arg(mTableName));
        query.bindValue(":id", mId);
        query.exec();
        query.next();
        
        if (query.value(0).toInt() == 0)
        {
            create();
        }
    }
    
    registerObject(this);
}

