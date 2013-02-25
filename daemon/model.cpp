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
#include <QSqlQuery>

Model::Model(const QString& tableName, QObject* parent)
: QObject(parent)
, mTableName(tableName)
{
    mId = QUuid::createUuid().toString();

    QSqlQuery query;
    query.prepare("INSERT INTO :tableName (_id) VALUES :id");
    query.bindValue(":tableName", mTableName);
    query.bindValue(":id", mId);
}

Model::Model(const QString& tableName, const QString& id, QObject* parent)
: QObject(parent)
, mTableName(tableName)
, mId(id)
{

}

Model::~Model()
{

}

void Model::remove()
{
    QSqlQuery query;
    query.prepare("DELETE :tableName WHERE _id=:id");
    query.bindValue(":tableName", mTableName);
    query.bindValue(":id", mId);
    query.exec();

    deleteLater();
}

QString Model::id() const
{
    return mId;
}

QVariant Model::getField(const QString& field)
{
    QSqlQuery query;
    query.prepare("SELECT :field FROM :tableName WHERE _id=:id");
    query.bindValue(":field", field);
    query.bindValue(":tableName", mTableName);
    query.bindValue(":id", mId);

    query.exec();
    if (query.next())
    {
        return query.value(0);
    }

    return QVariant();
}

void Model::saveField(const QString& field)
{
    QSqlQuery query;
    query.prepare("UPDATE :tableName SET :field=:value WHERE _id=:id");
    query.bindValue(":tableName", mTableName);
    query.bindValue(":field", field);
    query.bindValue(":value", property("field"));
    query.bindValue(":id", mId);
    query.exec();
}
