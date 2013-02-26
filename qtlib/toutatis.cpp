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
: com::noughmad::Toutatis("com.noughmad.Toutatis", "/Toutatis", QDBusConnection::sessionBus(), parent)
, d_ptr(new ToutatisPrivate)
{
    connect(this, SIGNAL(projectIdsChanged()), SLOT(updateProjects()));
}

Toutatis::~Toutatis()
{
    delete d_ptr;
}

void Toutatis::updateProjects()
{
    Q_D(Toutatis);
    updateModelList(d->projects, projectIds(), this);
    emit projectsChanged();
}
