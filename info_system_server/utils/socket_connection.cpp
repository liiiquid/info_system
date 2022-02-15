#include "socket_connection.h"

socket_connection::socket_connection(QTcpSocket* client):client(client)
{
    QObject::connect(this->client,SIGNAL(readyRead()),this,SLOT(read_from_client()));
}

socket_connection::~socket_connection()
{

}

void socket_connection::run()
{

}

message socket_connection::read_from_client()
{
    QByteArray client_msg = this->client->readAll();
    return message();
}

