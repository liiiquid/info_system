#include "message.h"

message::message()
{

}

const QString message::type_table[NUM_TYPE]  = {"登陆","查询","修改","注册","沟通","图片","服务器确认连接","需审核请求"};

message::message(/*int size,*/ int type,int sender, int send_type, int receiver, int receiver_type,QString content)
{
    /*this->size = size;*/
    this->type = type;
    this->sender = sender;
    this->send_type = send_type;
    this->receiver = receiver;
    this->receiver_type = receiver_type;
    this->content = content;
}
