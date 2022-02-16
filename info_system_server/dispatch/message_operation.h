#ifndef MESSAGE_OPERATION_H
#define MESSAGE_OPERATION_H
#include <QQueue>
#include "message.h"
#include "message_analysis.h"

class TServer;
class message_operation
{
public:
    message_operation(db_operation* db);
    message* msg_dispatch(message* msg);

public:
    message_analysis* msg_analysis;
};

#endif // MESSAGE_OPERATION_H
