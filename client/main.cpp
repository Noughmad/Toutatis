#include "toutatis_interface.h"
#include "client.h"

#include <QCoreApplication>
#include <QStringList>

#include <iostream>

using namespace std;
using namespace com::noughmad;

void printUsage()
{
    cout << "Usage: toutatis (start|stop) [arguments]" << endl;
    cout << "       toutatis <type> <command> [arguments]" << endl;
    cout << "       toutatis --help" << endl;
    cout << "       toutatis <type> --help" << endl;
    cout << endl;
    cout << "Possible types are 'project', 'task', 'event' or 'note'";
}

void project(Toutatis* t, const QStringList& args)
{
    if (args.isEmpty() || args.first() == "list")
    {
        foreach (const QString& name, t->projects().value())
        {
            cout << name.toStdString() << endl;
        }
    }
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
