#include "client.h"
#include "toutatis_interface.h"
#include "project_interface.h"
#include "task_interface.h"

using namespace com::noughmad;
using namespace com::noughmad::toutatis;

class Arguments
{
public:
    Arguments(QStringList& args);

    QString project;
    QString task;
    QString command;
    QStringList commandArguments;
    QString error;
};

Arguments::Arguments(QStringList& args)
{
    while (!args.isEmpty())
    {
        QString arg = args.takeFirst();
        if (arg == "--project")
        {
            if (args.isEmpty())
            {
                error = "No project specified";
                break;
            }

            project = args.takeFirst();
        }
        else if (arg == "--task")
        {
            if (args.isEmpty())
            {
                error = "No task specified";
                break;
            }

            task = args.takeFirst();
        }
        else
        {
            commandArguments << arg;
        }
    }

    if (!commandArguments.isEmpty())
    {
        command = commandArguments.takeFirst();
    }
}

Client::Client(QObject* parent) : QObject(parent)
{
    mDaemon = new Toutatis(
        "com.noughmad.Toutatis",
        "/Toutatis",
        QDBusConnection::sessionBus(),
        this);
}

Client::~Client()
{

}

bool Client::parseArguments(const QStringList& arguments)
{
    QStringList a = arguments;
    Arguments args(a);

    Project* project = getProject(args.project);
    Task* task = getTask(args.task, args.project);

    return false;
}

ComNoughmadToutatisProjectInterface* Client::getProject(const QString& name)
{
    if (name.isEmpty())
    {
        return 0;
    }

    Project* project = new Project(
        "com.noughmad.Toutatis",
        "/Project/" + name,
        QDBusConnection::sessionBus(),
        this);

    if (project->isValid())
    {
        return project;
    }

    qlonglong id = mDaemon->findProject(name).value();
    if (id > 0)
    {
        project = new Project(
            "com.noughmad.Toutatis",
            "/Project/" + QString::number(id),
            QDBusConnection::sessionBus(),
            this);

        return project->isValid() ? project : 0;
    }

    return 0;
}

ComNoughmadToutatisTaskInterface* Client::getTask(const QString& name, const QString& project)
{
    if (name.isEmpty())
    {
        return 0;
    }

    Task* task = new Task(
        "com.noughmad.Toutatis",
        "/Task/" + name,
        QDBusConnection::sessionBus(),
        this);

    if (task->isValid())
    {
        return task;
    }

    if (project.isEmpty())
    {
        return 0;
    }

    qlonglong id = mDaemon->findTask(project, name).value();
    if (id > 0)
    {
        task = new Task(
            "com.noughmad.Toutatis",
            "/Task/" + QString::number(id),
            QDBusConnection::sessionBus(),
            this);

        return task->isValid() ? task : 0;
    }

    return 0;
}