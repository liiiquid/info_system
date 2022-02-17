#include "t_socket.h"

t_socket::t_socket():QTcpSocket()
{

}

void t_socket::write_out(message *mg)
{
    if(mg)
    {
        this->write(message_serialization::serialize(mg));
        delete mg;
        qDebug() << "written successfully!!!";
    }else{
        qDebug() << "written failed...";
    }

}
