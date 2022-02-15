#include "message_operation.h"
#include "../m_network/tserver.h"
message_operation::message_operation(TServer* server)
{
    this->msg_analysis = new message_analysis(server);
}

message *message_operation::msg_dispatch(message *msg)
{
    if(msg->type == 7) return msg;
    return this->msg_analysis->msg_analysis(msg);

}
