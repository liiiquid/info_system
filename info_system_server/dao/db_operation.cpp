#include "db_operation.h"
#include "../utils/message_serialization.h"
QMutex mutex;
int get_field_num(QString sql)
{
    int res = 0;
    for(int i = 7; i < sql.length();i++)
    {
        if(sql[i] == ',') res++;
        else if(sql[i] == ' '){res++;break;}
    }
    return res;
}
db_operation::db_operation(QString driver, QString host, QString dbname, QString user, QString pwd)
{
    this->driver = driver;
    this->host = host;
    this->dbname = dbname;
    this->pwd = pwd;
    this->user = user;
}

db_operation::db_operation()
{

}

db_operation::~db_operation()
{

}
void db_operation::link()
{
    if(!db.isOpen())
    {
        db = QSqlDatabase::addDatabase(driver);
        db.setDatabaseName(dbname);
        db.setHostName(host);
        db.setUserName(user);
        db.setPassword(pwd);
        bool ok = db.open();
        if(!ok)
        {
            qDebug() <<"open database failed...";
        }
    }
}

void db_operation::no_link()
{
   QString connect = db.connectionName();
   db.close();
   db = QSqlDatabase();
   db.removeDatabase(connect);
}
QString db_operation::select(QString sql)
{
    mutex.lock();
    link();
   int field_size = get_field_num(sql);
    QSqlQuery query(sql);
    QString str = "";

    query.exec();
    if(query.isActive())
    {
        while (query.next()) {
            int i = 0;
            while(i < field_size)
            {
                  str += query.value(i).toString();i++;
                  str+='\t';
            }
            str+='\n';
        }
        str += '0' + query.size();
    }
    query.clear();
   no_link();
    mutex.unlock();
    return str;
}

int db_operation::update(QString sql)
{
    mutex.lock();
    link();
    QSqlQuery query(sql);
    query.exec();
    query.clear();
    no_link();
    mutex.unlock();
    return query.numRowsAffected() >= 0 ? query.numRowsAffected() : -1;

}
