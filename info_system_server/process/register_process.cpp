#include "register_process.h"
#include "../utils/message_serialization.h"
#include "../server.h"
register_process::register_process(TServer* server)
{
    this->socket_server=server;
}

message *register_process::reg_check(message *msg)
{
     return new message(7,msg->sender,msg->send_type,msg->receiver,msg->receiver_type,msg->content);
}
