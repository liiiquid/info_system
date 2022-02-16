#include "image_process.h"
#include "../dao/db_operation.h"
#include "../utils/message_serialization.h"
image_process::image_process(db_operation* db_operation)
{
    this->dbo = db_operation;
}

message *image_process::image_store(message *msg)
{
return nullptr;
}
