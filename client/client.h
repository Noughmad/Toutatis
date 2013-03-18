#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>

class Task;
class Project;
class Toutatis;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject* parent = 0);
    virtual ~Client();
    void parseArguments(const QString& project, const QString& task, const QString& command, const QStringList& args, bool create);
    

private:
    Project* getProject(const QString& name);
    Task* getTask(const QString& name, const QString& project);

private:
    Toutatis* mDaemon;
};

#endif // CLIENT_H
