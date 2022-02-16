#include "message_operation.h"
#include "../m_network/tserver.h"
message_operation::message_operation(db_operation* dbo)
{
    this->msg_analysis = new message_analysis(dbo);
}

message *message_operation::msg_dispatch(message *msg)
{
    qDebug() << "thread dispatch: " << QThread::currentThread() << ": is running...";
    if(msg->type == 7) return msg;
    return this->msg_analysis->msg_analysis(msg);

}
