#include "update_process.h"
#include "../server.h"
#include "../utils/message_serialization.h"
update_process::update_process(TServer* server)
{
    this->socket_server = server;
}

message *update_process::update_db(message *msg)
{
    if(msg->sender != msg->receiver)
        return new message(7,msg->sender,msg->send_type,msg->receiver,msg->receiver_type,msg->content);
    server* ser = this->socket_server->server_ori;
    int res = ser->dbo->update(msg->content);
    QString content = message_serialization::int2str(res);
    content += " 条记录修改成功!";
    return new message(11,0,0,msg->sender,msg->send_type,content);
}
