#include "toutatis_interface.h"
#include "client.h"

#include <QCoreApplication>
#include <QStringList>

#include <tclap/CmdLine.h>
#include <tclap/ValueArg.h>

#include <iostream>

using namespace std;
using namespace com::noughmad;

void printUsage()
{
    cout << "Usage: toutatis [--project <project>] [--task <task>] <command> [arguments]" << endl;
    cout << endl;
    cout << "Projects and tasks can be specified either by name or by id" << endl;
    cout << endl;
    cout << "Accepted commands and their arguments are:" << endl;
    cout << "  start [[--create] <task name>]" << endl;
    cout << "  stop" << endl;
    cout << "  status" << endl;
    cout << "  list-projects" << endl;
    cout << "  list-tasks" << endl;
    cout << "  remove" << endl;
    cout << "  add-event <type> <short message> [<longer message>]" << endl;
}

int main(int argc, char** argv)
{
    QString task;
    QString project;
    bool create = false;
    
    QString command;
    QStringList args;
    
    QCoreApplication app(argc, argv);
    
    try
    {
        TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
        
        TCLAP::ValueArg<std::string> projectArg("p", "project", "Project id or name", false, std::string(), "string", cmd);
        TCLAP::ValueArg<std::string> taskArg("t", "task", "Task id or name", false, std::string(), "string", cmd);
        
        TCLAP::SwitchArg createSwitch("c", "create", "Create task if it does not exist", cmd, false);
        TCLAP::UnlabeledValueArg<std::string> commandArg("command", "Toutatis command", true, "status", "string", cmd);
        
        TCLAP::UnlabeledMultiArg<std::string> commandArgumentsArg("args", "Command arguments", false, "list of strings", cmd);
        
        cmd.parse(argc, argv);
        
        project = QString::fromStdString(projectArg.getValue());
        task = QString::fromStdString(taskArg.getValue());
        create = createSwitch.getValue();
        command = QString::fromStdString(commandArg.getValue());
        
        foreach (const std::string& arg, commandArgumentsArg.getValue())
        {
            args << QString::fromStdString(arg);
        }
    }
    catch (TCLAP::ArgException &e) 
    { 
        // catch any exceptions
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
    
    qDebug() << project << task;
    Client client;
    client.parseArguments(project, task, command, args, create);

    return 0;
}
