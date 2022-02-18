#ifndef TSERVER_H
#define TSERVER_H
#include <QTcpServer>
#include <QTcpSocket>
#include "../thread_about/m_thread_pool.h"
#include "../thread_about/mthread.h"
#include <QMutex>

class server;
class TServer :public QTcpServer
{
    Q_OBJECT
public:
    TServer(server* server_ori);
    ~TServer();
    void incomingConnection(qintptr socketDescriptor);
    void add_client(int id, t_socket* socket);
    t_socket* get_client(int id);
    void add_unproced_message(message* msg);
    void remove_client(int id);
    void remove_client(t_socket* socket);
    void close_sockets();
public:
    m_thread_pool* thread_pool;
    QHash<int,t_socket*> socket_clients;

    QVector<message*> unproced_messages;
    server* server_ori;
signals:
    void show_new_connection(int id);
    void show_new_msg();
     void user_offline(int key);
};

#endif // TSERVER_H
