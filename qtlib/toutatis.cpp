#include "toutatis.h"
#include "project.h"

#include <QStringList>

class ToutatisPrivate
{
public:
    QList<Project*> projects;
};

Toutatis::Toutatis(QObject* parent)
: com::noughmad::Toutatis("com.noughmad.Toutatis", "/Toutatis", QDBusConnection::sessionBus(), parent)
{
    connect(this, projectsChanged, updateProjects);
}

Toutatis::~Toutatis()
{

}

void Toutatis::updateProjects()
{
    Q_D(Toutatis);
    QStringList newIds = projectIds();
    QStringList oldIds;
    foreach (Project* p, d->projects)
    {
        oldIds << p->id();
    }

    foreach (Project* p, d->projects)
    {
        if (!newIds.contains(p->id()))
        {
            d->projects.removeAll(p);
            delete p;
        }
    }

    foreach (QString id, newIds)
    {
        if (!oldIds.contains(id))
        {
            Project* p = new Project(this);
            d->projects << p;
        }
    }
}
