#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QVector>

#include "model.h"

class Project;
class Task : public Model
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.toutatis.Task")
    Q_PROPERTY(QString projectId READ projectId WRITE setProjectId NOTIFY projectIdChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QDateTime lastStart READ lastStart WRITE setLastStart)
    Q_PROPERTY(QStringList noteIds READ noteIds NOTIFY noteIdsChanged)
    Q_PROPERTY(QStringList eventIds READ eventIds NOTIFY eventIdsChanged)
    Q_PROPERTY(qlonglong duration READ duration)

public:
    explicit Task(const QString& id, Project* parent = 0);
    explicit Task(Project* parent = 0);
    virtual ~Task();

    void init();

    T_STRING_FIELD(projectId, ProjectId);
    T_STRING_FIELD(name, Name);
    T_STRING_FIELD(status, Status);

    bool isActive() const;
    void setActive(bool active);

    QDateTime lastStart() const;
    void setLastStart(const QDateTime& start);

    qlonglong duration() const;

    QStringList noteIds() const;
    QStringList eventIds() const;

public slots:
    QString addNote(const QString& title, const QString& contents);
    QString addEvent(const QString& eventType, const QDateTime& start, const QDateTime& end = QDateTime(), const QString& message = QString());

    void start();
    void stop();

signals:
    void projectIdChanged(const QString& projectId);
    void nameChanged(const QString& name);
    void statusChanged(const QString& status);
    void activeChanged(bool active);
    void noteIdsChanged();
    void eventIdsChanged();
};

#endif // TASK_H
