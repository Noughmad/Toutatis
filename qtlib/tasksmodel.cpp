#include "tasksmodel.h"
#include "toutatis.h"

TasksModel::TasksModel(QObject* parent): QStandardItemModel(parent)
{
    mDaemon = new Toutatis(this);
}

TasksModel::~TasksModel()
{

}
