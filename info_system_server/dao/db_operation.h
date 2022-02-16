#ifndef DB_OPERATION_H
#define DB_OPERATION_H
#include "message.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVector>
#include <QMutex>
class db_operation
{
public:
    db_operation(QString driver, QString host, QString dbname, QString user, QString pwd);
    db_operation();
    ~db_operation();
    QMutex mutex;
    QString select(QString sql);
    int update(QString sql);
public:
    QSqlDatabase db;
};

#endif // DB_OPERATION_H
