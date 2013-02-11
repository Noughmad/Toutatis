#ifndef Toutatis_H
#define Toutatis_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

class Toutatis : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.Toutatis")
    Q_PROPERTY(QString currentTask READ currentTask WRITE startTracking NOTIFY currentTaskChanged)
    Q_PROPERTY(QStringList projects READ projects NOTIFY projectsChanged)

public:
    Toutatis(QObject* parent = 0);
    virtual ~Toutatis();

    QStringList projects() const;

public slots:
    QString createProject(const QString& name, const QString& client = QString());

    QString currentTask() const;
    void startTracking(const QString& id);
    void startTracking(const QString& project, const QString& task, bool create);
    void stopTracking();
    bool isTracking() const;

    QString findProject(const QString& name);
    QString findTask(const QString& project, const QString& task);

signals:
    void projectsChanged();
    void currentTaskChanged(const QString& id);

private:
    void createTables();

private:
    QSqlDatabase mDatabase;
    QString mCurrentTask;
};

#endif // Toutatis_H
