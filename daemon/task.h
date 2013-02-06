#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QStringList>
#include <QDateTime>

class Project;
class Task : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.toutatis.Task")
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QDateTime lastStart READ lastStart WRITE setLastStart)

public:
    explicit Task(qlonglong id, Project* parent = 0);
    virtual ~Task();

    QString name() const;
    void setName(const QString& name);

    QString status() const;
    void setStatus(const QString& status);

    bool isActive() const;
    void setActive(bool active);

    QDateTime lastStart() const;
    void setLastStart(const QDateTime& start);

    qlonglong duration() const;

    qlonglong id() const;

    QString fullObjectPath() const;


public slots:
    void remove();

    void start();
    void stop();

signals:
    void nameChanged(const QString& name);
    void statusChanged(const QString& status);
    void activeChanged(bool active);

private:
    qlonglong mId;
};

#endif // TASK_H
