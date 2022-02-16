#include "select_process.h"
#include "../dao/db_operation.h"
#include "../utils/message_serialization.h"
select_process::select_process(db_operation* dbo)
{
    this->dbo = dbo;
}

message *select_process::select_db(message *msg)
{
    message_serialization serial;
    QString result_select = dbo->select(msg->content);
    return new message(msg->type,0,0,msg->sender,msg->send_type,result_select);
}
