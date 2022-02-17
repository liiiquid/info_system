#include "update_process.h"
#include "../server.h"
#include "../utils/message_serialization.h"
update_process::update_process(db_operation* dbo)
{
    this->dbo = dbo;
}

message *update_process::update_db(message *msg)
{
    if(msg->send_type == 2)/*the information that update the user self will be allowed else in all other conditions will be permitted*/
        return new message(7,msg->sender,msg->send_type,msg->receiver,msg->receiver_type,msg->content);
    int res = dbo->update(msg->content);
    QString content = message_serialization::int2str(res);
    content += " 条记录修改成功!";
    return new message(msg->type,msg->sender,msg->send_type,msg->receiver,msg->receiver_type,content);
}
