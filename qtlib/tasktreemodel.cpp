#include "tasktreemodel.h"
#include "tasktreemodel_p.h"

#include "toutatis.h"
#include "project.h"
#include "task_interface.h"

using namespace com::noughmad;
using namespace com::noughmad::toutatis;

void TaskTreeModelPrivate::loadProjects()
{
}

TaskTreeModel::TaskTreeModel(QObject* parent)
: QAbstractItemModel(parent)
, d_ptr(new TaskTreeModelPrivate(this))
{
    Q_D(TaskTreeModel);
    d->daemon = new Toutatis(
        "com.noughmad.Toutatis",
        "/Toutatis",
        QDBusConnection::sessionBus(),
        this);
    connect(d->daemon, SIGNAL(projectsChanged()), SLOT(loadProjects()));
    d->loadProjects();
}

TaskTreeModel::~TaskTreeModel()
{
    delete d_ptr;
}

bool TaskTreeModel::hasChildren(const QModelIndex& parent) const
{
    if (parent.isValid() && parent.parent().isValid())
    {
        return false;
    }

    return true;
}

QVariant TaskTreeModel::data(const QModelIndex& index, int role) const
{

}

int TaskTreeModel::columnCount(const QModelIndex& parent) const
{

}

int TaskTreeModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const TaskTreeModel);
    if (!parent.isValid())
    {
        return d->projects.size();
    }

    else if (!parent.parent().isValid())
    {
        Project* p = static_cast<Project*>(parent.internalPointer());
        return d->tasks[p].size();
    }

    return 0;
}

QModelIndex TaskTreeModel::parent(const QModelIndex& child) const
{
    Q_D(const TaskTreeModel);
    Task* t = static_cast<Task*>(child.internalPointer());

    Project* p = 0;
    for (QHash<Project*, QList<Task*> >::const_iterator it = d->tasks.constBegin(); it != d->tasks.constEnd(); ++it)
    {
        if (it.value().contains(t))
        {
            p = it.key();
            break;
        }
    }
    if (p)
    {
        return index(d->projects.indexOf(p), 0);
    }

    p = static_cast<Project*>(child.internalPointer());
    if (d->projects.contains(p))
    {
        return index(0, 0);
    }
}

QModelIndex TaskTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_D(const TaskTreeModel);
    if (!parent.isValid())
    {
        // We want top-level items, I.E. projects

        return createIndex(row, column, d->projects[row]);
    }
    else if (!parent.parent().isValid())
    {
        // Second-order items are tasks

        Project* project = static_cast<Project*>(parent.internalPointer());
        return createIndex(row, column, d->tasks[project][row]);
    }
    else
    {
        // TODO: Show events and notes here
    }

    return QModelIndex();
}
