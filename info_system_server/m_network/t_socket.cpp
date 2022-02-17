#include "t_socket.h"
#include "tserver.h"
#include "../server.h"
t_socket::t_socket(TServer* server) : QTcpSocket()
{
    this->server = server;
    this->msgs = new QQueue<message*>;
    this->dbo = new db_operation("QMYSQL","localhost","stu_mgr","root","root");
    this->msg_opr = new message_operation(this->dbo,server);
}

t_socket::~t_socket()
{

}

bool t_socket::event(QEvent *e)
{
    switch(e->type())
    {
        case QEvent::User+1:
        {
            m_event* event = (m_event*)e;
            event->mg.print();
            write(message_serialization::serialize(&event->mg));
        }
            return true;
    default:
        break;
    }
    return QTcpSocket::event(e);
}
void t_socket::close_socket()
{
    while(msgs->size() >0)
    {
        delete msgs->dequeue();
    }
    delete msg_opr;

    delete dbo;
    this->close();
    emit socket_close();
}
void t_socket::read()
{
    QByteArray array =  this->readAll();
    message* mg = message_serialization::unserialize(array);
    msgs->enqueue(mg);
    emit readok();
}

void t_socket::write_process()
{
        message* mg = msgs->dequeue();
        message* result = msg_opr->msg_dispatch(mg);
        if(result->type == 7) {this->server->add_unproced_message(result); delete mg;return;}
        if(result->type == 12) {this->server->add_client(result->sender,this);}
        this->write(message_serialization::serialize(result));
        delete mg;
        delete result;



}


