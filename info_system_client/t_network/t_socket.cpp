#include "t_socket.h"

t_socket::t_socket():QTcpSocket()
{

}

void t_socket::write_out(message *mg)
{
    qDebug() << "current socket thread: " << QThread::currentThreadId() << "is running";
    qDebug() << mg;
 this->write(message_serialization::serialize(mg));
    delete mg;
    qDebug() << "written successfully!!!";
}
