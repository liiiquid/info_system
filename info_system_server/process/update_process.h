#ifndef UPDATE_PROCESS_H
#define UPDATE_PROCESS_H
#include "message.h"

class db_operation;
class update_process
{
public:
    update_process(db_operation* dbo);
    db_operation* dbo;
    message* update_db(message* msg);
};

#endif // UPDATE_PROCESS_H
