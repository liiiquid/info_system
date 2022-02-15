#ifndef REGISTER_PROCESS_H
#define REGISTER_PROCESS_H
#include "message.h"

class TServer;
class register_process
{
public:
    register_process(TServer* server);
    message* reg_check(message* msg);
    TServer* socket_server;
};

#endif // REGISTER_PROCESS_H
