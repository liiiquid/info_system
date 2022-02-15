#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H
#include "message.h"

class TServer;
class image_process
{
public:
    image_process(TServer* server);
    TServer* socket_server;
    message* image_store(message* msg);
};


#endif // IMAGE_PROCESS_H
