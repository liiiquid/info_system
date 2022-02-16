#include "register_process.h"
#include "../utils/message_serialization.h"
#include "../dao/db_operation.h"
register_process::register_process(db_operation* dbo)
{
    this->dbo=dbo;
}

message *register_process::reg_check(message *msg)
{
     return new message(7,msg->sender,msg->send_type,msg->receiver,msg->receiver_type,msg->content);
}
