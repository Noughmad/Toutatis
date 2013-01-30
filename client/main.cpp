#include "toutatis_interface.h"

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

    Toutatis* toutatis = new Toutatis(
        "com.noughmad.Toutatis",
        "/Toutatis",
        QDBusConnection::sessionBus(),
        0
    );

    QStringList args = app.arguments();
    args.takeFirst();
    qDebug() << args;

    if (args.first() == "project")
    {
        args.takeFirst();
        project(toutatis, args);
        return 0;
    }

    if (args.first() == "stop" && args.size() == 1)
    {
        toutatis->stopTracking().waitForFinished();
        return 0;
    }

    if (args.first() == "start" && args.size() >= 3)
    {
        if (args[1] == "--create")
        {
            toutatis->startTask(args[2], args[3], true).waitForFinished();
        }
        else
        {
            toutatis->startTask(args[1], args[2]).waitForFinished();
        }
        return 0;
    }

    printUsage();
    return 0;
}
