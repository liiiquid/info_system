#ifndef MTHREAD_H
#define MTHREAD_H
#include <QThread>

#include "m_network/t_socket.h"

class TServer;
class mthread : public QThread
{
    Q_OBJECT
public:
    mthread(qintptr socketDescriptor,TServer* server);
/*virtual funciton*/
    void run();

public slots:
    void exit_thread();
signals:
    void thread_over(mthread*);
public:
    qintptr socketDescriptor;
    TServer* server;
    t_socket* socket;
};

#endif // MTHREAD_H
