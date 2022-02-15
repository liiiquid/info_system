#include "message_analysis.h"
#include "../m_network/tserver.h"
#include "../server.h"

message_analysis::message_analysis(TServer* server)
{
    this->server_socket = server;/*used for getting the socket table of clients*/
    this->login = new login_process(server);
    this->select = new select_process(server);
    this->talk = new talk_process(server);
    this->reg = new register_process(server);
    this->update = new update_process(server);
    this->server_ori = this->server_socket->server_ori;
    this->image = new image_process(server);
}

/*analysis the message*/
message *message_analysis::msg_analysis(message *msg)
{

    if(msg->type == 0)
        return login->login_check(msg);
    else if(msg->type == 1)
        return select->select_db(msg);
    else if(msg->type == 2)
        return update->update_db(msg);
    else if(msg->type == 3)
        return reg->reg_check(msg);
    else if(msg->type == 4)
         return talk->transfer_client(msg);
     else if(msg->type == 5)
        return image->image_store(msg);
    else return new message(99,0,0,0,0,0);
}
