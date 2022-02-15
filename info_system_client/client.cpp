#include "client.h"

client::client()
{

}

client::~client()
{
delete cur_user;
}


void client::read_process()
{

}

void client::show_data()
{

}



bool client::client_login(message msg)
{
    QVector<QString> information = message_serialization::analysis_content(msg.content);
    for(int i = 0; i < information.size();i++)
    {
        qDebug() << information[i];
    }
    return false;
}

bool client::client_register(message msg)
{
    return false;
}



void client::show_client(QWidget *parent)
{
    this->parent = parent;
    init_main_list(parent);
}

void client::show_main_list(QWidget *parent)
{

}

void client::init_main_list(QWidget* parent)
{

}

