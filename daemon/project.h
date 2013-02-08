#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QStringList>
#include <QVector>

class Project : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.toutatis.Project")
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(QVector<qlonglong> tasks READ tasks NOTIFY tasksChanged)

public:
    explicit Project(qlonglong id, QObject* parent = 0);
    virtual ~Project();

    QString name() const;
    void setName(const QString& name);

    QString client() const;
    void setClient(const QString& name);

    QVector<qlonglong> tasks() const;

    qlonglong id() const;

public slots:
    qlonglong createTask(const QString& task);

    void remove();
    qlonglong findTask(const QString& task);

signals:
    void tasksChanged();

    void removed();
    void nameChanged(const QString& name);
    void clientChanged(const QString& name);

private:
    qlonglong mId;
};

#endif // PROJECT_H
