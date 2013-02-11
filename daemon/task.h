#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QStringList>
#include <QDateTime>
#include <QVector>

class Project;
class Task : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.toutatis.Task")
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(bool active READ isActive WRITE setActive NOTIFY activeChanged)
    Q_PROPERTY(QDateTime lastStart READ lastStart WRITE setLastStart)
    Q_PROPERTY(QStringList notes READ notes NOTIFY notesChanged)
    Q_PROPERTY(QStringList events READ events NOTIFY eventsChanged)

public:
    explicit Task(const QString& id, Project* parent = 0);
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

    QString id() const;

    QStringList notes() const;
    QStringList events() const;

public slots:
    QString addNote(const QString& title, const QString& contents);
    void removeNote(const QString& title);
    QString addEvent(const QString& eventType, qlonglong start, qlonglong end, const QString& title, const QString& message = QString());
    void remove();

    void start();
    void stop();

signals:
    void removed();
    void nameChanged(const QString& name);
    void statusChanged(const QString& status);
    void activeChanged(bool active);
    void notesChanged();
    void eventsChanged();

private:
    QString mId;
};

#endif // TASK_H
