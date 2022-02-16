#ifndef T_SOCKET_H
#define T_SOCKET_H
#include <QTcpSocket>
#include "../message.h"
#include "../utils/message_serialization.h"
#include <QDebug>
#include <QThread>
class t_socket : public QTcpSocket
{
    Q_OBJECT
public:
    t_socket();
public slots:
     void write_out(message*);
};

#endif // T_SOCKET_H
