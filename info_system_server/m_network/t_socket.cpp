#include "t_socket.h"
#include "tserver.h"
#include "../server.h"
t_socket::t_socket(TServer* server) : QTcpSocket()
{
    this->server = server;
    this->msgs = new QQueue<message*>;
    QString str = "thread";
    str += i;
    this->dbo->db.cloneDatabase(this->server->server_ori->dbo,);
    this->dbo = new db_operation("QMYSQL","localhost","stu_mgr","root","root");
    this->msg_opr = new message_operation(this->dbo);
}

t_socket::~t_socket()
{

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
        this->server->add_client(12,this);
        this->write(message_serialization::serialize(result));
        delete mg;
        delete result;
}


