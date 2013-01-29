#include "toutatis.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>

Toutatis::Toutatis(QObject* parent) : QObject(parent)
{
    mDatabase = QSqlDatabase::addDatabase("QSQLITE");
}

Toutatis::~Toutatis()
{
    mDatabase.close();
}

QStringList Toutatis::projects() const
{
    QSqlQuery query;
    query.prepare("SELECT name FROM projects WHERE visible = 1");
    query.exec();

    QStringList list;
    while(query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}

void Toutatis::createProject(const QString& name, const QString& client)
{
    QSqlQuery query;
    query.prepare("INSERT INTO projects (name, client, visible) VALUES (:name, :client, :visible)");
    query.bindValue(":name", name);
    query.bindValue(":client", client);
    query.bindValue(":visible", 1);
    query.exec();
}

QStringList Toutatis::tasks(const QString& project)
{
    QSqlQuery query;
    query.prepare("SELECT name FROM tasks WHIRE project = :project");
    query.bindValue(":project", project);

    QStringList list;
    while(query.next())
    {
        list << query.value(0).toString();
    }
    return list;
}


void Toutatis::createTask(const QString& project, const QString& task)
{
    QSqlQuery query;
    query.prepare("INSERT INTO tasks (name, project) VALUES (:name, :project)");
    query.bindValue(":name", task);
    query.bindValue(":projectId", project);
    query.exec();
}

void Toutatis::renameTask(const QString& project, const QString& task, const QString& name)
{

}

void Toutatis::removeTask(const QString& project, const QString& task)
{

}

void Toutatis::currentProjectAndTask(QString& project, QString& task)
{

}

void Toutatis::startTask(const QString& project, const QString& task)
{

}

void Toutatis::stop()
{

}

void Toutatis::stopTask(const QString& project, const QString& task)
{

}

void Toutatis::addNote(const QString& project, const QString& task, const QString& note)
{

}

void Toutatis::addEvent(const QString& project, const QString& task, const QString& eventType, const QString& message)
{

}

void Toutatis::output()
{
}

#include "toutatis.moc"
