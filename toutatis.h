#ifndef Toutatis_H
#define Toutatis_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

class Toutatis : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.Toutatis")

public:
    Toutatis(QObject* parent = 0);
    virtual ~Toutatis();


public slots:
    QStringList projects() const;
    void createProject(const QString& name, const QString& client = QString());

    QString currentProjectAndTask(QString& task);
    void startTask(const QString& project, const QString& task);
    void stopTask(const QString& project, const QString& task);
    void stopTracking();
    bool isTracking();

    void addNote(const QString& project, const QString& task, const QString& note);
    void addEvent(const QString& project, const QString& task, const QString& eventType, qint64 start, qint64 end, const QString& message = QString());

signals:
    void projectChanged(const QString& name);
    void taskChanged(const QString& project, const QString& task);
    void currentTaskChanged(const QString& project, const QString& task);

private:
    void output();

private:
    QSqlDatabase mDatabase;

    QString mCurrentProject;
    QString mCurrentTask;

    void createTables();
};

#endif // Toutatis_H
