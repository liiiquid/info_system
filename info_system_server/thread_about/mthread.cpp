#include "mthread.h"
#include "../m_network/tserver.h"

mthread::mthread(qintptr socketDescriptor,TServer* server) : QThread()
{
    this->socketDescriptor = socketDescriptor;
    this->server = server;
}

void mthread::run()
{
    qDebug() << "thread " << this << ": is running...";
    socket = new QTcpSocket();
    if(!socket->setSocketDescriptor(socketDescriptor)){this->exit();if(socket) delete socket;return;}
    QObject::connect(socket,SIGNAL(disconnected()),this,SLOT(close_socket()));
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(read()));
    QObject::connect(this,SIGNAL(readok()),this,SLOT(write()));
    serial = new message_serialization();
    msg_opr = new message_operation(this->server);
    msgs = new QQueue<message*>;
    this->exec();
}

/*slot function*/

void mthread::close_socket()
{
    socket->close();
    delete socket;
    this->quit();
    this->exit();
    emit thread_over(this);
}
void mthread::read()
{
    this->cache = socket->readAll();
    msgs->enqueue(serial->unserialize(this->cache));
    qDebug()<< "message queue size: " << msgs->size();
    emit readok();
}

void mthread::write()
{
    message* msg = this->msgs->dequeue();
    message* res = msg_opr->msg_dispatch(msg);
    if(res->type == 6) {this->server->add_client(res->receiver,this->socket); emit new_client();} /*type 6 means that server has accepted this client and tell the client*/
    else if(res->type == 7) {this->server->add_unproced_message(res); emit new_message();/*need procoessed by the administrator...*/}
    socket->write(serial->serialize(res));
    delete res;
    delete msg;
}

