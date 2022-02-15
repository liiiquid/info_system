#include "select_process.h"
#include "../server.h"
#include "../utils/message_serialization.h"
select_process::select_process(TServer* server)
{
    this->socket_server = server;
}

message *select_process::select_db(message *msg)
{
    message_serialization serial;
    server* ser = socket_server->server_ori;
    db_operation* dbo = ser->dbo;
    QString result_select = dbo->select(msg->content);
    return new message(10,0,0,msg->sender,msg->send_type,result_select);
}
