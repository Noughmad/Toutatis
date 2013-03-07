#include "toutatis.h"
#include "project.h"
#include "global.h"

#include <QStringList>

class ToutatisPrivate
{
public:
    QList<Project*> projects;
    com::noughmad::Toutatis* interface;
};

Toutatis::Toutatis(QObject* parent)
: QObject(parent)
, d_ptr(new ToutatisPrivate)
{
    Q_D(Toutatis);
    qDebug() << "Creating a Toutatis";
    
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.interface()->isServiceRegistered(Service))
    {
        dbus.interface()->startService(Service);
    }
    
    d->interface = new com::noughmad::Toutatis(Service, "/Toutatis", QDBusConnection::sessionBus(), this);
    
    connect(d->interface, SIGNAL(projectIdsChanged()), SLOT(updateProjects()));
    updateProjects();
}

Toutatis::~Toutatis()
{
    delete d_ptr;
}

QList< Project* > Toutatis::projects() const
{
    Q_D(const Toutatis);
    return d->projects;
}

void Toutatis::startTracking(const QString& id)
{
    Q_D(Toutatis);
    d->interface->startTracking(id);
}

void Toutatis::stopTracking()
{
    Q_D(Toutatis);
    d->interface->stopTracking();
}

QString Toutatis::findProject(const QString& name)
{
    Q_D(Toutatis);
    return d->interface->findProject(name).value();
}

QString Toutatis::findTask(const QString& project, const QString& name)
{
    Q_D(Toutatis);
    return d->interface->findTask(project, name).value();
}

void Toutatis::updateProjects()
{
    Q_D(Toutatis);
    updateModelList(d->projects, d->interface->projectIds(), this);
    emit projectsChanged();
}
