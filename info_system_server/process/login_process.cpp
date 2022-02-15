#include "login_process.h"
#include "../utils/message_serialization.h"
#include "../server.h"
login_process::login_process(TServer* server)
{
    this->socket_server = server;
}

message *login_process::login_check(message *msg)
{
    message_serialization serial;
    QString sql = "select user_id,password from users where user_id = ";
    QVector<QString> contents = serial.split(msg->content,QString(SEG_ONE_LINE));
    QString id = contents[0];
    sql += id;
    sql += " and password = '";
    QString password = contents[1];
    sql += password;
    sql += "';";
    server* ser = this->socket_server->server_ori;
    QString content = ser->dbo->select(sql);
    QVector<QString> res = serial.analysis_content(content);
    if(res.size() == 2) return new message(6,0,0,msg->sender,msg->send_type,"登陆成功");
    else return new message(0,0,0,0,0,"账号不存在或用户名密码错误");
}

