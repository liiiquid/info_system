#include "talk_process.h"
#include "../server.h"
#include "../utils/message_serialization.h"
talk_process::talk_process(db_operation* dbo)
{
    this->dbo = dbo;
}

message *talk_process::transfer_client(message *msg)
{

    return new message(4,0,0,0,0,"不支持离线发送");
}
