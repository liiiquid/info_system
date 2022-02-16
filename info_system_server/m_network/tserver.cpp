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
    this->thread_pool->start(th);
}

TServer::~TServer()
{
}
void TServer::add_client(int id, t_socket *socket)/*after login successfully, emit new connection tell the server there is a new client connected*/
{
    mutex_add_client.lock();
    this->socket_clients.insert(id,socket);
    emit show_new_connection(id);
    mutex_add_client.unlock();
}

t_socket *TServer::get_client(int id)
{
    if(this->socket_clients[id]) return this->socket_clients[id];
    else return nullptr;
}

void TServer::add_unproced_message(message *msg)
{
    mutex_add_unproced.lock();
    this->unproced_messages.push_back(msg);
    mutex_add_unproced.unlock();
}
