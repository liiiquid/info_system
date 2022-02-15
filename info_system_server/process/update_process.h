#ifndef UPDATE_PROCESS_H
#define UPDATE_PROCESS_H
#include "message.h"

class TServer;
class update_process
{
public:
    update_process(TServer* server);
    TServer* socket_server;
    message* update_db(message* msg);
};

#endif // UPDATE_PROCESS_H
