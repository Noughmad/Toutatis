#ifndef Toutatis_H
#define Toutatis_H

#include <QtCore/QObject>

class Toutatis : public QObject
{
Q_OBJECT
public:
    Toutatis(QObject* parent = 0);
    virtual ~Toutatis();

private slots:
    void output();
};

#endif // Toutatis_H
