#ifndef LOGIN_PROCESS_H
#define LOGIN_PROCESS_H
#include "message.h"

class db_operation;
class login_process
{
public:
    login_process(db_operation* dbo);
    db_operation* dbo;
    message* login_check(message* msg);
};

#endif // LOGIN_PROCESS_H
