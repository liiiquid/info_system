#include "talk_process.h"
#include "../server.h"
#include "../utils/message_serialization.h"
talk_process::talk_process(db_operation* dbo,TServer* server)
{
    this->dbo = dbo;
    this->server = server;
}

message *talk_process::transfer_client(message *msg)
{
    m_event* event = new m_event(QEvent::Type(QEvent::User + 1));
    event->mg = message(msg->type,msg->sender,msg->send_type,msg->receiver,msg->receiver_type,msg->content);
    QCoreApplication::postEvent(this->server->get_client(msg->receiver),event);
    return new message(4,0,0,0,0,"消息发送成功");
}
