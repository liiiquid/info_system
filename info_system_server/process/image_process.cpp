#include "image_process.h"
#include "../server.h"
#include "../utils/message_serialization.h"
image_process::image_process(TServer* server)
{
    this->socket_server = server;
}

message *image_process::image_store(message *msg)
{
return nullptr;
}
