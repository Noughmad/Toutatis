#ifndef PROJECT_H
#define PROJECT_H

#include "model.h"

#include <QStringList>

class Project : public Model
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.noughmad.toutatis.Project")
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(QStringList tasks READ tasks NOTIFY tasksChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged)

public:
    explicit Project(const QString& id, QObject* parent = 0);
    explicit Project(QObject* parent = 0);
    virtual ~Project();

    void init();

    T_STRING_FIELD(name, Name);
    T_STRING_FIELD(client, Client);

    bool isVisible() const;
    void setVisible(bool visible);

    QStringList tasks() const;

public slots:
    QString createTask(const QString& task);
    QString findTask(const QString& task);

signals:
    void tasksChanged();
    void nameChanged(const QString& name);
    void clientChanged(const QString& name);
    void visibleChanged(bool visible);

#ifdef DBUS_CPP_2_XML
public slots:
    void remove();
signals:
    void removed();
#endif
};

#endif // PROJECT_H
