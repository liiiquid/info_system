#include "db_operation.h"

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
    this->db = QSqlDatabase::addDatabase(driver);
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

db_operation::db_operation()
{

}

db_operation::~db_operation()
{
    this->db.close();
}

QString db_operation::select(QString sql)
{
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
    return str;
}

int db_operation::update(QString sql)
{
    QSqlQuery query(sql);
    query.exec();
    if(query.isActive())
        return query.numRowsAffected();
    else return -1;

}
