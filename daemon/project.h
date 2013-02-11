#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QStringList>

class Project : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.toutatis.Project")
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(QStringList tasks READ tasks NOTIFY tasksChanged)

public:
    explicit Project(const QString& id, QObject* parent = 0);
    virtual ~Project();

    QString name() const;
    void setName(const QString& name);

    QString client() const;
    void setClient(const QString& name);

    QStringList tasks() const;

    QString id() const;

public slots:
    QString createTask(const QString& task);

    void remove();
    QString findTask(const QString& task);

signals:
    void tasksChanged();

    void removed();
    void nameChanged(const QString& name);
    void clientChanged(const QString& name);

private:
    QString mId;
};

#endif // PROJECT_H
