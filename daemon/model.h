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


#ifndef MODEL_H
#define MODEL_H

#include <QObject>


class Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)

public:
    explicit Model(const QString& tableName, const QString& id, QObject* parent = 0);
    explicit Model(const QString& tableName, QObject* parent = 0);
    virtual ~Model();

    QString id() const;

public Q_SLOTS:
    void remove();

protected:
    void saveField(const QString& field);
    QVariant getField(const QString& field);

private:
    QString mTableName;
    QString mId;
};

#endif // MODEL_H
