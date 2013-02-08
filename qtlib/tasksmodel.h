#ifndef TASKSMODEL_H
#define TASKSMODEL_H

#include <QStandardItemModel>

class Toutatis;
class TasksModel : public QStandardItemModel
{
    Q_OBJECT

public:
    explicit TasksModel(QObject* parent = 0);
    virtual ~TasksModel();

private:
    Toutatis* mDaemon;
};

#endif // TASKSMODEL_H
