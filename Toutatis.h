#ifndef Toutatis_H
#define Toutatis_H

#include <QtCore/QObject>

class Toutatis : public QObject
{
Q_OBJECT
public:
    Toutatis();
    virtual ~Toutatis();
private slots:
    void output();
};

#endif // Toutatis_H
