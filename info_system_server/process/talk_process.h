#ifndef TALK_PROCESS_H
#define TALK_PROCESS_H
#include "message.h"

class TServer;
class talk_process
{
public:
    talk_process(TServer* server);
    message* transfer_client(message* msg);
public:
    TServer* socket_server;
};

#endif // TALK_PROCESS_H
