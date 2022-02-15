#ifndef SELECT_PROCESS_H
#define SELECT_PROCESS_H
#include "message.h"

class TServer;
class select_process
{
public:
    select_process(TServer* server);
    TServer* socket_server;
    message* select_db(message* msg);
};

#endif // SELECT_PROCESS_H
