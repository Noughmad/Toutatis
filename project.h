#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QStringList>

class Project : public QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.Toutatis.Project")
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString client READ client WRITE setClient NOTIFY clientChanged)

public:
    explicit Project(qlonglong id, QObject* parent = 0);
    virtual ~Project();

    QString name() const;
    void setName(const QString& name);

    QString client() const;
    void setClient(const QString& name);

    qlonglong id() const;

    QString fullObjectPath() const;


public slots:

    QStringList tasks();
    void createTask(const QString& task);

    void remove();

signals:
    void nameChanged(const QString& name);
    void clientChanged(const QString& name);

private:
    qlonglong mId;
};

#endif // PROJECT_H
