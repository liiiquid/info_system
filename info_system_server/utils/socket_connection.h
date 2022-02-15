#ifndef SOCKET_CONNECTION_H
#define SOCKET_CONNECTION_H
#include <QRunnable>
#include <QTcpSocket>
#include "message.h"
#include "message_serialization.h"
class socket_connection : public QRunnable, public QObject
{
    Q_OBJECT
public:
    socket_connection(QTcpSocket* client);
    ~socket_connection();
    void run();
public:
    QTcpSocket* client;
public slots:
    message read_from_client();

};

#endif // SOCKET_CONNECTION_H
