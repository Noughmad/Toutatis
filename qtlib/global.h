#ifndef TOUTATIS_GLOBAL_H
#define TOUTATIS_GLOBAL_H

#include <QList>
#include <QStringList>
#include <QHash>
#include <QPointer>

static const char* Service = "com.noughmad.Toutatis";

template <class T> void updateModelList(QList< T* >& list, const QStringList& ids, QObject* parent);
template <class T> T* getOrCreateModel(QObject* object, const QString& id, QObject* parent);

template <class T> void updateModelList(QList<T*>& list, const QStringList& ids, QObject* parent)
{
    QStringList oldIds;
    foreach (T* t, list)
    {
        oldIds << t->id();
    }
    
    foreach (T* t, list)
    {
        if (!ids.contains(t->id()))
        {
            list.removeAll(t);
            delete t;
        }
    }
    
    foreach (QString id, ids)
    {
        if (!oldIds.contains(id))
        {
            T* t = new T(id, parent);
            list << t;
        }
    }
}

template <class T> T* getOrCreateModel(QObject* object, const QString& id, QObject* parent)
{
    if (T* t = qobject_cast<T*>(object))
    {
        if (t->id() == id)
        {
            return t;
        }
    }
    
    return new T(id, parent);
}


#endif // TOUTATIS_GLOBAL_H