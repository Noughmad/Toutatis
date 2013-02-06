#include "client.h"
#include "toutatis_interface.h"

using namespace com::noughmad;

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
    return false;
}
