#include "login_process.h"
#include "../utils/message_serialization.h"
#include "../dao/db_operation.h"
login_process::login_process(db_operation* dbo)
{
    this->dbo = dbo;
}

message *login_process::login_check(message *msg)
{
    QString content = dbo->select(msg->content);
    QVector<QString> res = message_serialization::analysis_content(content);
    if(res.size() == 2) return new message(12,msg->sender,msg->send_type,msg->receiver,msg->receiver,"登陆成功");
    else return new message(13,msg->sender,msg->send_type,msg->receiver,msg->receiver,"账号不存在或用户名密码错误");
}

