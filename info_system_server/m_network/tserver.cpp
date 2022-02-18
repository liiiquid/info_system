#include "tserver.h"
#include "../server.h"
QMutex mutex_add_client;
QMutex mutex_add_unproced;
QMutex mutex_remove_client;
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

void TServer::remove_client(int id)/*eliminated...*/
{
    //mutex_remove_client.lock();
    if(this->socket_clients[id]) this->socket_clients.remove(id);
    qDebug() << "current online num: " << message_serialization::int2str(this->socket_clients.size());
    emit user_offline(id);
    //mutex_remove_client.unlock();
}
void TServer::remove_client(t_socket* socket)
{
    mutex_add_client.lock();
    qDebug() << "current executing thread: " << QThread::currentThread();
   for(QHash<int,t_socket*>::Iterator i = this->socket_clients.begin();i != this->socket_clients.end();i++)
   {
       if(i.value() == socket)
       {
           int id = i.key();
           this->socket_clients.erase(i); emit user_offline(id);break;
       }
   }
   mutex_add_client.unlock();
}
void TServer::close_sockets()
{
    for(QHash<int,t_socket*>::Iterator i = this->socket_clients.begin(); i != this->socket_clients.end();i++)
        i.value()->close();
}

void TServer::add_unproced_message(message *msg)
{
    mutex_add_unproced.lock();
    this->unproced_messages.push_back(msg);
    emit show_new_msg();
    mutex_add_unproced.unlock();
}
