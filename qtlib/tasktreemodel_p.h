#include <QStringList>
#include <QHash>

class Project;
class Toutatis;
class TaskTreeModel;

class TaskTreeModelPrivate
{
public:
    QStringList projectIds;
    TaskTreeModelPrivate(TaskTreeModel* q) : q_ptr(q) {}
    Toutatis* daemon;

    void loadProjects();
    void loadProject(Project* p);

private:
    TaskTreeModel* const q_ptr;
    Q_DECLARE_PUBLIC(TaskTreeModel);
};