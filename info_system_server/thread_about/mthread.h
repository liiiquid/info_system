#ifndef MTHREAD_H
#define MTHREAD_H
#include <QThread>
#include <QQueue>
#include <QTcpSocket>
#include "../utils/message_serialization.h"
#include "../dispatch/message_operation.h"

class TServer;
class mthread : public QThread
{
    Q_OBJECT
public:
    mthread(qintptr socketDescriptor,TServer* server);
/*virtual funciton*/
    void run();

public slots:
    void read();
    void write();
    void close_socket();
signals:
    void readok();
    void thread_over(mthread* thread);
    void new_message();
    void new_client();
public:
    qintptr socketDescriptor;
    TServer* server;
    QTcpSocket* socket;
    QQueue<message*> *msgs;
    QByteArray cache;
    message_operation* msg_opr;
    message_serialization* serial;
};

#endif // MTHREAD_H
