#ifndef TASKTREEMODEL_H
#define TASKTREEMODEL_H

#include <QtCore/qabstractitemmodel.h>

class TaskTreeModelPrivate;

class TaskTreeModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TaskTreeModel(QObject* parent = 0);
    virtual ~TaskTreeModel();

    virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
    virtual bool hasChildren(const QModelIndex& parent = QModelIndex()) const;
    virtual QModelIndex parent(const QModelIndex& child) const;
    virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;

private:
    TaskTreeModelPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(TaskTreeModel)
    Q_PRIVATE_SLOT(d_func(), void loadProjects());
};

#endif // TASKTREEMODEL_H
