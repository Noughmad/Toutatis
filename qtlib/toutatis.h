#ifndef TOUTATIS_H
#define TOUTATIS_H

#include "toutatis_interface.h"

class Toutatis : public com::noughmad::Toutatis
{
    Q_OBJECT

public:
    explicit Toutatis(QObject* parent = 0);
    virtual ~Toutatis();
};

#endif // TOUTATIS_H
