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
    Q_PROPERTY(QString project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QDateTime lastStart READ lastStart WRITE setLastStart)
    Q_PROPERTY(QStringList notes READ notes NOTIFY notesChanged)
    Q_PROPERTY(QStringList events READ events NOTIFY eventsChanged)
    Q_PROPERTY(qlonglong duration READ duration)

public:
    explicit Task(const QString& id, Project* parent = 0);
    explicit Task(Project* parent = 0);
    virtual ~Task();

    void init();

    T_STRING_FIELD(project, Project);
    T_STRING_FIELD(name, Name);
    T_STRING_FIELD(status, Status);

    bool isActive() const;
    void setActive(bool active);

    QDateTime lastStart() const;
    void setLastStart(const QDateTime& start);

    qlonglong duration() const;

    QStringList notes() const;
    QStringList events() const;

public slots:
    QString addNote(const QString& title, const QString& contents);
    QString addEvent(const QString& eventType, const QDateTime& start, const QDateTime& end = QDateTime(), const QString& message = QString());

    void start();
    void stop();

signals:
    void projectChanged(const QString& projectId);
    void nameChanged(const QString& name);
    void statusChanged(const QString& status);
    void activeChanged(bool active);
    void notesChanged();
    void eventsChanged();
};

#endif // TASK_H
