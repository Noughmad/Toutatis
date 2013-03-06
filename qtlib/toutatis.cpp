#include "toutatis.h"
#include "project.h"
#include "global.h"

#include <QStringList>

class ToutatisPrivate
{
public:
    QList<Project*> projects;
};

Toutatis::Toutatis(QObject* parent)
: com::noughmad::Toutatis(Service, "/Toutatis", QDBusConnection::sessionBus(), parent)
, d_ptr(new ToutatisPrivate)
{
    connect(this, SIGNAL(projectIdsChanged()), SLOT(updateProjects()));
    
    QDBusConnection dbus = QDBusConnection::sessionBus();
    if (!dbus.interface()->isServiceRegistered(Service))
    {
        dbus.interface()->startService(Service);
    }
    
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

void Toutatis::updateProjects()
{
    Q_D(Toutatis);
    updateModelList(d->projects, projectIds(), this);
    emit projectsChanged();
}
