#ifndef TALK_PROCESS_H
#define TALK_PROCESS_H
#include "message.h"

class db_operation;
class talk_process
{
public:
    talk_process(db_operation* dbo);
    message* transfer_client(message* msg);
public:
    db_operation* dbo;
};

#endif // TALK_PROCESS_H
