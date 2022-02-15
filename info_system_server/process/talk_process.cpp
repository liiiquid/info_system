#include "talk_process.h"
#include "../server.h"
#include "../utils/message_serialization.h"
talk_process::talk_process(TServer* server)
{
    this->socket_server = server;
}

message *talk_process::transfer_client(message *msg)
{
    message_serialization serial;
    if(this->socket_server->socket_clients[msg->receiver])
    {   this->socket_server->socket_clients[msg->receiver]->write(serial.serialize(msg));
        return new message(4,0,0,0,0,"发送成功");
    }
    return new message(4,0,0,0,0,"不支持离线发送");
}
