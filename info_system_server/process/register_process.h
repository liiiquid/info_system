#ifndef REGISTER_PROCESS_H
#define REGISTER_PROCESS_H
#include "message.h"

class db_operation;
class register_process
{
public:
    register_process(db_operation* dbo);
    message* reg_check(message* msg);
    db_operation* dbo;
};

#endif // REGISTER_PROCESS_H
