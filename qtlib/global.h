#ifndef TOUTATIS_GLOBAL_H
#define TOUTATIS_GLOBAL_H

#include <QList>
#include <QStringList>

static const char* Service = "com.noughmad.Toutatis";

template <class T> void updateModelList(QList< T* >& list, const QStringList& ids, QObject* parent);

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

#endif // TOUTATIS_GLOBAL_H