#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class ComNoughmadToutatisInterface;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject* parent = 0);
    virtual ~Client();
    bool parseArguments(const QStringList& arguments);

private:
    ComNoughmadToutatisInterface* mDaemon;
};

#endif // CLIENT_H
