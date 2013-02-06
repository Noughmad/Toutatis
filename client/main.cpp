#include "toutatis_interface.h"
#include "client.h"

#include <QCoreApplication>
#include <QStringList>

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
    QCoreApplication app(argc, argv);
    Client client;
    bool ok = client.parseArguments(app.arguments());

    if (!ok)
    {
        printUsage();
    }

    return 0;
}
