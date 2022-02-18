#ifndef T_SOCKET_H
#define T_SOCKET_H
#include <QTcpSocket>

#include <QDebug>
#include <QThread>

#include "../message.h"
#include "../utils/message_serialization.h"
#include "../mevent/m_event.h"
class t_socket : public QTcpSocket
{
    Q_OBJECT
public:
    t_socket();
public slots:
     void write_out(message*);
     void close_self();
     bool event(QEvent*);
};

#endif // T_SOCKET_H
