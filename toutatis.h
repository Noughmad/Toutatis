#ifndef Toutatis_H
#define Toutatis_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

class Toutatis : public QObject
{
    Q_OBJECT

public:
    Toutatis(QObject* parent = 0);
    virtual ~Toutatis();


public slots:
    QStringList projects() const;
    void createProject(const QString& name, const QString& client = QString());

    QStringList tasks(const QString& project);
    void createTask(const QString& project, const QString& task);
    void removeTask(const QString& project, const QString& task);
    void renameTask(const QString& project, const QString& task, const QString& name);

    void currentProjectAndTask(QString& project, QString& task);
    void startTask(const QString& project, const QString& task);
    void stopTask(const QString& project, const QString& task);
    void stop();

    void addNote(const QString& project, const QString& task, const QString& note);
    void addEvent(const QString& project, const QString& task, const QString& eventType, const QString& message);

private:
    void output();

private:
    QSqlDatabase mDatabase;
};

#endif // Toutatis_H
