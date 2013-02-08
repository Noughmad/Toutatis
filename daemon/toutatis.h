#ifndef Toutatis_H
#define Toutatis_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

class Toutatis : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.Toutatis")
    Q_PROPERTY(qlonglong currentTask READ currentTask WRITE startTracking NOTIFY currentTaskChanged)
    Q_PROPERTY(QVector<qlonglong> projects READ projects NOTIFY projectsChanged)

public:
    Toutatis(QObject* parent = 0);
    virtual ~Toutatis();


public slots:
    QVector<qlonglong> projects() const;
    qlonglong createProject(const QString& name, const QString& client = QString());

    qlonglong currentTask() const;
    void startTracking(qlonglong task);
    void startTracking(const QString& project, const QString& task, bool create);
    void stopTracking();
    bool isTracking() const;

    qlonglong findProject(const QString& name);
    qlonglong findTask(const QString& project, const QString& task);

signals:
    void projectsChanged();
    void currentTaskChanged(qlonglong id);

private:
    void output();

private:
    QSqlDatabase mDatabase;

    qlonglong mCurrentTask;

    void createTables();
};

#endif // Toutatis_H
