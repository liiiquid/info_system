#include "tserver.h"
#include "../server.h"
TServer::TServer(server* server) : QTcpServer()
{
    this->thread_pool = new m_thread_pool(50);
    this->server_ori = server;


}

void TServer::incomingConnection(qintptr socketDescriptor)
{
    mthread* th = new mthread(socketDescriptor,this);
    QObject::connect(th,&mthread::thread_over,this->thread_pool,&m_thread_pool::close_thread);
    QObject::connect(th,&mthread::new_message,this->server_ori,&server::update_msg_list);
    QObject::connect(th,&mthread::new_client,this->server_ori,&server::update_online_num);
    this->thread_pool->start(th);
}

TServer::~TServer()
{
    for(QHash<int,QTcpSocket*>::Iterator i = this->socket_clients.begin();i != this->socket_clients.end();i++) if(i.value()) delete i.value();
}
void TServer::add_client(int id, QTcpSocket *socket)
{
    mutex.lock();
    this->socket_clients.insert(id,socket);
    emit show_new_connection(id);
    mutex.unlock();
}

QTcpSocket *TServer::get_client(int id)
{
    if(this->socket_clients[id]) return this->socket_clients[id];
    else return nullptr;
}

void TServer::add_unproced_message(message *msg)
{
    this->unproced_messages.push_back(msg);
}
