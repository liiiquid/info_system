#include "message_analysis.h"
#include "../m_network/tserver.h"
#include "../server.h"

message_analysis::message_analysis(db_operation* dbo,TServer* server)
{
    this->server = server;
    this->dbo = dbo;/*used for getting the socket table of clients*/
    this->login = new login_process(dbo);
    this->select = new select_process(dbo);
    this->talk = new talk_process(dbo,server);
    this->reg = new register_process(dbo);
    this->update = new update_process(dbo);
    this->image = new image_process(dbo);
}

/*analysis the message*/
message *message_analysis::msg_analysis(message *msg)
{
    if(msg->type == 0)
        return login->login_check(msg);
    else if(msg->type == 10 || msg->type == 16 || msg->type == 17 || msg->type == 18 || msg->type == 19)
        return select->select_db(msg);
    else if(msg->type == 2 || msg->type == 11)
        return update->update_db(msg);
    else if(msg->type == 3)
        return reg->reg_check(msg);
    else if(msg->type == 4)
        return talk->transfer_client(msg);
    else if(msg->type == 5)
        return image->image_store(msg);
    else return new message(99,0,0,0,0,0);
}
