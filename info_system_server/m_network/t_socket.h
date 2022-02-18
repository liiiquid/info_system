#ifndef T_SOCKET_H
#define T_SOCKET_H
#include <QTcpSocket>
#include "../utils/message_serialization.h"
#include "../dispatch/message_operation.h"
#include "../dao/db_operation.h"
#include <QThread>
#include <QQueue>
#include "../mevent/m_event.h"
class TServer;
class t_socket : public QTcpSocket
{
    Q_OBJECT
public:
    t_socket(TServer* server);
    ~t_socket();
    QQueue<message*> *msgs;
    message_operation* msg_opr;
    TServer* server;
    db_operation* dbo;
    static QMutex mutex;
    bool event(QEvent*);
    int id;
signals:
    void readok();
    void socket_close();
public slots:
    void read();
    void write_process();
    void close_socket();
};

#endif // T_SOCKET_H
