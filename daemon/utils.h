#ifndef UTILS_H
#define UTILS_H

#include <QString>

class QSqlQuery;

namespace Utils
{
    QString slug(const QString& text);
    QStringList stringList(QSqlQuery& query);
}

#endif // UTILS_H
