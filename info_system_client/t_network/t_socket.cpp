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

void t_socket::write_out_2(QVector<message *>* mgs)
{
    if(mgs->size()>0)
    {
        this->write(message_serialization::serialize_2(mgs));
        qDebug() << "write_out_2 written successfully!!!";
    }
    for(int i  = 0; i < mgs->size();i++)
    {
        delete mgs->at(i);
    }
    delete mgs;
}

void t_socket::close_self()
{
    this->disconnectFromHost();
}

bool t_socket::event(QEvent *e)
{
     switch(e->type())
     {
        case QEvent::User+1:
        {
            this->disconnectFromHost();
            return true;
        }
         break;
     default:
         break;
     }

     return QTcpSocket::event(e);
}
