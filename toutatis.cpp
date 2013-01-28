#include "toutatis.h"

#include <QTimer>
#include <iostream>

Toutatis::Toutatis(QObject* parent) : QObject(parent)
{
    QTimer* timer = new QTimer(this);
    connect( timer, SIGNAL(timeout()), SLOT(output()) );
    timer->start( 1000 );
}

Toutatis::~Toutatis()
{}

void Toutatis::output()
{
    std::cout << "Hello World!" << std::endl;
}

#include "toutatis.moc"
