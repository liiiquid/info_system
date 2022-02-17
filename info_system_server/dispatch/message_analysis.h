#ifndef MESSAGE_ANALYSIS_H
#define MESSAGE_ANALYSIS_H
#include "message.h"
#include "../dao/db_operation.h"
#include "../process/login_process.h"
#include "../process/register_process.h"
#include "../process/select_process.h"
#include "../process/talk_process.h"
#include "../process/update_process.h"
#include "../process/image_process.h"
class TServer;
class message_analysis
{
public:
    message_analysis(db_operation* dbo,TServer* server);/*used for getting the socket table of clients*/
    message* msg_analysis(message* msg);

private:
    db_operation* dbo;
    login_process* login;
    register_process* reg;
    select_process* select;
    talk_process* talk;
    update_process* update;
    TServer* server;
    image_process* image;
};

#endif // MESSAGE_ANALYSIS_H
