#ifndef MTHREAD_H
#define MTHREAD_H
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QHostAddress>
#include "../mevent/m_event.h"
#include "message.h"
#include "t_network/t_socket.h"
#include "utils/message_serialization.h"

class client;
class mthread : public QThread
{
    Q_OBJECT
public:
    mthread(client* client);
    ~mthread();

    void run();

    QByteArray shared;
    client* client_ptr = nullptr;
    t_socket* socket = nullptr;
    void close_thread();
signals:
    void read_ok();
    void process_ok();
    void connect_failed();
    void connect_success();
    void reconnect();
    void user_exit();
public slots:
    void read();
    void process();
    void quit_thread();

};

#endif // MTHREAD_H
