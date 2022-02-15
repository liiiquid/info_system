#ifndef LOGIN_PROCESS_H
#define LOGIN_PROCESS_H
#include "message.h"

class TServer;
class login_process
{
public:
    login_process(TServer* server);
    TServer* socket_server;
    message* login_check(message* msg);
};

#endif // LOGIN_PROCESS_H
