#include "client.h"
#include "toutatis_interface.h"
#include "project_interface.h"
#include "task_interface.h"
#include "../qtlib/project.h"
#include "../qtlib/toutatis.h"
#include "../qtlib/task.h"

#include <iostream>

using namespace std;

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
    mDaemon = new Toutatis(this);
}

Client::~Client()
{

}

bool Client::parseArguments(const QStringList& arguments)
{
    QStringList a = arguments;
    a.removeFirst();
    Arguments args(a);

    Project* project = getProject(args.project);
    Task* task = getTask(args.task, args.project);
    
    QTextStream stream(stdout);
        
    if (args.command == "list-projects")
    {
        if (mDaemon->projects().isEmpty())
        {
            stream << "No projects" << endl;
        }
        else
        {
            foreach (Project* p, mDaemon->projects())
            {
                stream << p->name();
                if (p->client().isEmpty())
                {
                    stream << " (" << p->client() << ')' << endl;
                }
            }
        }
        return true;
    }
    else if (args.command == "list-tasks")
    {
        if (!project)
        {
            qWarning() << "No project specified";
        }
        else if (project->tasks().isEmpty())
        {
            stream << "No tasks for project \"" << project->name() << "\"" << endl;
        }
        else
        {
            foreach (Task* t, project->tasks())
            {
                stream << t->name();
                if (!t->status().isEmpty())
                {
                    stream << " (" << t->status() << ')' << endl;
                }
            }
        }
        return true;
    }

    return false;
}

Project* Client::getProject(const QString& name)
{
    if (name.isEmpty())
    {
        return 0;
    }

    Project* project = new Project(name, this);

    if (project->isValid())
    {
        return project;
    }

    QString id = mDaemon->findProject(name).value();
    if (!id.isEmpty())
    {
        project = new Project(id, this);
        return project->isValid() ? project : 0;
    }

    return 0;
}

Task* Client::getTask(const QString& name, const QString& project)
{
    if (name.isEmpty())
    {
        return 0;
    }

    Task* task = new Task(name, this);

    if (task->isValid())
    {
        return task;
    }

    if (project.isEmpty())
    {
        return 0;
    }

    QString id = mDaemon->findTask(project, name).value();
    if (!id.isEmpty())
    {
        task = new Task(id, this);
        return task->isValid() ? task : 0;
    }

    return 0;
}