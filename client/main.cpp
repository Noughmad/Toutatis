#include <QCoreApplication>
#include <QStringList>

#include "toutatis_interface.h"

using namespace com::noughmad;

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    Toutatis* toutatis = new Toutatis(
        "com.noughmad.Toutatis",
        "/Toutatis",
        QDBusConnection::sessionBus(),
        0
    );

    foreach (const QString& project, toutatis->projects().value())
    {
        qDebug() << project;
    }
    return 0;
}
