#include "project.h"
#include "projectadaptor.h"
#include "task.h"
#include "utils.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDBusConnection>

Project::Project(const QString& id, QObject* parent)
: Model(parent)
{
    initialize<Project, ProjectAdaptor>(id);
    
    foreach (const QString& taskId, taskIds())
    {
        Task* t = new Task(taskId, this);
        connect (t, SIGNAL(removed()), SIGNAL(taskIdsChanged()));
    }
}

Project::Project(QObject* parent)
: Model(parent)
{
    initialize<Project, ProjectAdaptor>();
    
    foreach (const QString& taskId, taskIds())
    {
        Task* t = new Task(taskId, this);
        connect (t, SIGNAL(removed()), SIGNAL(taskIdsChanged()));
    }
}

Project::~Project()
{

}

T_DEF_STRING_FIELD(Project, name, Name)
T_DEF_STRING_FIELD(Project, client, Client)

bool Project::isVisible() const
{
    return getField("visible").toBool();
}

void Project::setVisible(bool visible)
{
    saveField("visible", visible);
}

QStringList Project::taskIds() const
{
    return getList("Task", "projectId");
}

QString Project::createTask(const QString& task)
{
    Task* t = new Task(this);
    t->setName(task);

    emit taskIdsChanged();
    return t->id();
}

QString Project::findTask(const QString& task)
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM Task WHERE project=:project AND name=:name");
    query.bindValue(":project", id());
    query.bindValue(":name", task);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}
