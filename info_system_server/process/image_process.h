#ifndef IMAGE_PROCESS_H
#define IMAGE_PROCESS_H
#include "message.h"

class db_operation;
class image_process
{
public:
    image_process(db_operation* dbo);
    db_operation* dbo;
    message* image_store(message* msg);
};


#endif // IMAGE_PROCESS_H
