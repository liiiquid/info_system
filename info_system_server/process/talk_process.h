#ifndef TALK_PROCESS_H
#define TALK_PROCESS_H
#include "message.h"

class TServer;
class db_operation;
class talk_process
{
public:
    talk_process(db_operation* dbo,TServer* server);
    message* transfer_client(message* msg);
public:
    db_operation* dbo;
    TServer* server;
};

#endif // TALK_PROCESS_H
