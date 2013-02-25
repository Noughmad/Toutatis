#ifndef TOUTATIS_H
#define TOUTATIS_H

#include "toutatis_interface.h"
#include <QList>

class Project;
class ToutatisPrivate;

class Toutatis : public com::noughmad::Toutatis
{
    Q_OBJECT
    Q_PROPERTY(QList<Project*> projects READ projects NOTIFY projectsChanged)

public:
    explicit Toutatis(QObject* parent = 0);
    virtual ~Toutatis();

    QList<Project*> projects();

public Q_SLOTS:
    void updateProjects();

private:
    ToutatisPrivate* const d_ptr;
    Q_DECLARE_PRIVATE(Toutatis)
};

#endif // TOUTATIS_H
