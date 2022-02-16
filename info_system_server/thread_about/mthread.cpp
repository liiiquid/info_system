#include "mthread.h"
#include "../m_network/tserver.h"

mthread::mthread(qintptr socketDescriptor,TServer* server) : QThread()
{
     qDebug() << "thread mthread:: " << QThread::currentThread() << ": is running...";
    this->socketDescriptor = socketDescriptor;
    this->server = server;
}

void mthread::run()
{
    qDebug() << "thread child: " << QThread::currentThread() << ": is running...";
    t_socket* socket = new t_socket(this->server);
    this->socket = socket;
    if(!socket->setSocketDescriptor(socketDescriptor)){this->exit();if(socket) delete socket;return;}
    QObject::connect(socket,SIGNAL(disconnected()),socket,SLOT(close_socket()));
    QObject::connect(socket,SIGNAL(readyRead()),socket,SLOT(read()));
    QObject::connect(socket,SIGNAL(readok()),socket,SLOT(write_process()));
    QObject::connect(socket,SIGNAL(socket_close()),this,SLOT(exit_thread()));
    this->exec();
}
/*slot function*/
void mthread::exit_thread()
{
    delete socket;
    this->exit();
    emit thread_over(this);
}

