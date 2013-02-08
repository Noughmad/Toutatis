#include "toutatis.h"

Toutatis::Toutatis(QObject* parent)
: com::noughmad::Toutatis("com.noughmad.Toutatis", "/Toutatis", QDBusConnection::sessionBus(), parent)
{

}

Toutatis::~Toutatis()
{

}