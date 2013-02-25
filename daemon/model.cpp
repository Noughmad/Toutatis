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
#include <QDebug>

class ObjectStore
{
public:
    QMap<QString, Model*> objects;
};

Q_GLOBAL_STATIC(ObjectStore, store)

Model::Model(const QString& tableName, QObject* parent)
: QObject(parent)
, mTableName(tableName)
{
    mId = QUuid::createUuid().toString();
    mId.remove(mId.size()-1, 1);
    mId.remove(0, 1);
    mId.replace(QRegExp("[^A-Za-z0-9]"), "_");

    QSqlQuery query;
    query.prepare("INSERT INTO " + mTableName + " (_id) VALUES (:id)");
    query.bindValue(":id", mId);

    bool ok = query.exec();
    Q_ASSERT(ok);

    registerObject(this);
}

Model::Model(const QString& tableName, const QString& id, QObject* parent)
: QObject(parent)
, mTableName(tableName)
, mId(id)
{
    registerObject(this);
}

Model::~Model()
{

}

void Model::remove()
{
    QSqlQuery query;
    query.prepare("DELETE " + mTableName + " WHERE _id=:id");
    query.bindValue(":id", mId);
    query.exec();

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
    query.prepare("UPDATE " + mTableName + " SET " + field + "=:value WHERE _id=:id");
    query.bindValue(":value", value);
    query.bindValue(":id", mId);
    query.exec();
}

QStringList Model::getList(const QString& table, const QString& key) const
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM " + table + " WHERE " + key + "=:id");
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
