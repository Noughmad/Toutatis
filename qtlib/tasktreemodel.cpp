#include "tasktreemodel.h"

#include "toutatis.h"
#include "project.h"
#include "task.h"
#include "task_interface.h"

class TaskTreeModelPrivate 
{
public:
    Toutatis daemon;
};

TaskTreeModel::TaskTreeModel(QObject* parent)
: QAbstractItemModel(parent)
, d_ptr(new TaskTreeModelPrivate)
{

}

TaskTreeModel::~TaskTreeModel()
{
    delete d_ptr;
}

QModelIndex TaskTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    Q_D(const TaskTreeModel);
    if (parent.isValid())
    {
        Project* p = static_cast<Project*>(parent.internalPointer());
        return createIndex(row, column, p->tasks().at(row));
    }
    else
    {
        return createIndex(row, column, d->daemon.projects().at(row));
    }
}

bool TaskTreeModel::hasChildren(const QModelIndex& parent) const
{
    return !parent.isValid();
}

int TaskTreeModel::rowCount(const QModelIndex& parent) const
{
    Q_D(const TaskTreeModel);
    if (!parent.isValid())
    {
        return d->daemon.projects().size();
    }
    else
    {
        return static_cast<Project*>(parent.internalPointer())->tasks().size();
    }
}

QModelIndex TaskTreeModel::parent(const QModelIndex& child) const
{
    Q_D(const TaskTreeModel);
    Task* t = static_cast<Task*>(child.internalPointer());
    
    if (d->daemon.projects().contains(static_cast<Project*>(child.internalPointer())))
    {
        return QModelIndex();
    }
    
    QList<Project*> projects = d->daemon.projects();
    for (int i = 0; i < projects.size(); ++i)
    {
        if (projects[i]->tasks().contains(t))
        {
            return index(i, 0);
        }
    }
    
    return QModelIndex();
}

int TaskTreeModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
    {
        // Project: name, client
        return 2;
    }
    else
    {
        // Task: name, status, duration, lastStart
        return 4;
    }
}

QVariant TaskTreeModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (index.parent().isValid())
        {
            Task* task = static_cast<Task*>(index.internalPointer());
            switch (index.column())
            {
                case 0:
                    return task->name();
                    
                case 1:
                    return task->status();
                    
                case 2:
                    return task->duration();
                    
                case 3:
                    return task->lastStart();
            }
        }
        else
        {
            Project* project = static_cast<Project*>(index.internalPointer());
            switch (index.column())
            {
                case 0:
                    return project->name();
                    
                case 1:
                    return project->client();
            }
        }
    }
    
    return QVariant();
}
