#ifndef TOUTATIS_H
#define TOUTATIS_H

#include "toutatis_interface.h"
#include "qtatis_export.h"

#include <QList>

class Project;
class ToutatisPrivate;

class QTATIS_EXPORT Toutatis : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Project*> projects READ projects NOTIFY projectsChanged)

public:
    explicit Toutatis(QObject* parent = 0);
    virtual ~Toutatis();

    QList<Project*> projects() const;

public Q_SLOTS:
    void updateProjects();
    void startTracking(const QString& id);
    void stopTracking();
    
    QString findProject(const QString& name);
    QString findTask(const QString& project, const QString& name);
    
signals:
    void projectsChanged();

private:
    ToutatisPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Toutatis)
};

#endif // TOUTATIS_H
