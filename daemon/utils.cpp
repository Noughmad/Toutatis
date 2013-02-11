#include "utils.h"
#include <QStringList>
#include <QVariant>
#include <QSqlQuery>

QStringList Utils::stringList(QSqlQuery& query)
{
    QStringList list;
    while (query.next())
    {
        list << query.value(0).toString();
    }
}
