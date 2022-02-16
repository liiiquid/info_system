#ifndef SELECT_PROCESS_H
#define SELECT_PROCESS_H
#include "message.h"

class db_operation;
class select_process
{
public:
    select_process(db_operation* dbo);
    db_operation* dbo;
    message* select_db(message* msg);
};

#endif // SELECT_PROCESS_H
