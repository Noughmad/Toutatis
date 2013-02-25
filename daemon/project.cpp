#include "project.h"
#include "projectadaptor.h"
#include "task.h"
#include "utils.h"

#include <QSqlQuery>
#include <QVariant>
#include <QDBusConnection>

Project::Project(const QString& id, QObject* parent)
: Model("projects", id, parent)
{
    init();
}

Project::Project(QObject* parent)
: Model("projects", parent)
{
    init();
}

void Project::init()
{
    new ProjectAdaptor(this);

    QDBusConnection dbus = QDBusConnection::sessionBus();
    bool ok = dbus.registerObject("/Project/" + id(), this);

    foreach (const QString& taskId, tasks())
    {
        Task* t = new Task(taskId, this);
        connect (t, SIGNAL(removed()), SIGNAL(tasksChanged()));
    }
}

Project::~Project()
{
    qDebug() << "Deleting project" << id();
}

QString Project::name() const
{
    return getField("name").toString();
}

void Project::setName(const QString& name)
{
    saveField("name", name);
}

QString Project::client() const
{
    return getField("client").toString();
}

void Project::setClient(const QString& client)
{
    saveField("client", client);
}

QStringList Project::tasks() const
{
    return getList("tasks", "project");
}

QString Project::createTask(const QString& task)
{
    Task* t = new Task(this);
    t->setName(task);

    emit tasksChanged();
    return t->id();
}

QString Project::findTask(const QString& task)
{
    QSqlQuery query;
    query.prepare("SELECT _id FROM tasks WHERE project=:project AND name=:name");
    query.bindValue(":project", id());
    query.bindValue(":name", task);
    query.exec();

    if (query.next())
    {
        return query.value(0).toString();
    }

    return QString();
}
