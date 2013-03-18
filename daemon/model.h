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
#include <QHash>
#include <QSqlQuery>
#include <QDBusConnection>

#define T_REF_FIELD(type, name, upperName)      \
type name() const;                              \
void set##upperName(const type& name);          \

#define T_STRING_FIELD(name, upperName) T_REF_FIELD(QString, name, upperName)
#define T_DATE_FIELD(name, upperName) T_REF_FIELD(QDateTime, name, upperName)

#define T_DEF_STRING_FIELD(cls, name, upperName)                \
QString cls::name() const {return getField(#name).toString();}  \
void cls::set##upperName(const QString& name) {                 \
    saveField(#name, name);                                     \
    emit name##Changed(name);                                   \
}

#define T_DEF_DATE_FIELD_X(cls, name, upperName, signalName)                                            \
QDateTime cls::name() const {return QDateTime::fromMSecsSinceEpoch(getField(#name).toLongLong());}      \
void cls::set##upperName(const QDateTime& name) {                                                       \
    saveField(#name, name.toMSecsSinceEpoch());                                                         \
    emit signalName();                                                                                  \
}

#define T_DEF_DATE_FIELD(csl, name, upperName) T_DEF_DATE_FIELD(cls, name, upperName, name##Changed)

class Model : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id)

public:
    explicit Model(QObject* parent = 0);
    virtual ~Model();

    QString id() const;

    template <class T>
    static T* findObject(const QString& id);

    static void registerObject(Model* object);
    static Model* findObject(const QString& id);
    
    static QHash<QString,Model*> allObjects();
    
    template <class T>
    static void createTable();
    
    static QString cleanId(const QString& uuid);
    
public slots:
    void remove();

signals:
    void removed();

protected:
    template <class T, class Adaptor> void initialize(const QString& id = QString());
    void saveField(const QString& field, const QVariant& value);
    QVariant getField(const QString& field) const;
    QStringList getList(const QString& table, const QString& key) const;
    
private:
    static void createTable(const QMetaObject& meta);
    void setupId(const QString& id);
    void create();

private:
    QString mTableName;
    QString mId;
};

template <class T>
T* Model::findObject(const QString& id)
{
    return qobject_cast<T*>(findObject(id));
}

template <class T>
void Model::createTable()
{
    createTable(T::staticMetaObject);
}

template <class T, class Adaptor>
void Model::initialize(const QString& id)
{
    setupId(id);
    
    new Adaptor(qobject_cast<T*>(this));
    QDBusConnection dbus = QDBusConnection::sessionBus();
    QString path = QString("/%1/%2").arg(mTableName).arg(cleanId(mId));
    dbus.registerObject(path, this);
}



#endif // MODEL_H
