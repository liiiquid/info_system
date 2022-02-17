#include "mthread.h"
#include "../client.h"
mthread::mthread(client* cl)
{
    this->client_ptr = cl;
}

mthread::~mthread()
{

}

void mthread::run()
{
    qDebug() << "the child process: " << QThread::currentThreadId() <<  "is running...";

    QHostAddress addr;
    addr.setAddress("127.0.0.1");
    this->socket = new t_socket();
    while(1)
    {
        this->socket->connectToHost(addr,12345);
        if(this->socket->waitForConnected())
        {
        QObject::connect(this->socket,&t_socket::readyRead,this,&mthread::read);
        QObject::connect(this,&mthread::read_ok, this,&mthread::process);
        QObject::connect(this,&mthread::process_ok,this->client_ptr,&client::show_data);
        QObject::connect(this->socket,&t_socket::disconnected,this,&mthread::quit_thread);
        QObject::connect(this->client_ptr,SIGNAL(write_to_server(message*)),this->socket,SLOT(write_out(message*)));
        this->client_ptr->isconnected = 1;
        qDebug() << "connect success!!!";
        break;
        }
        else{
            qDebug() << "connect failed...";
            this->client_ptr->read_msgs.push_back(new message(99,0,0,0,0,QString("服务器连接失败...")));emit connect_failed();this->client_ptr->isconnected = 0;}
        sleep(1);
    }
    this->exec();
}

void mthread::read()
{
    this->shared = this->socket->readAll();
    qDebug() << this->shared;
    emit read_ok();
}

void mthread::process()
{
    message* tm = message_serialization::unserialize(this->shared);
    qDebug() <<message_serialization::int2str( tm->sender) << " talk with you: " << tm->content;
    this->client_ptr->mutex_read->lock();
    this->client_ptr->read_msgs.enqueue(tm);
    this->client_ptr->mutex_read->unlock();
    emit process_ok();
}

void mthread::quit_thread()
{
    this->quit();
    socket->close();
    delete this->socket;
    this->client_ptr->isconnected= 0;
    this->client_ptr->read_msgs.push_back(new message(99,0,0,0,0,QString("服务器连接失败...")));
    emit connect_failed();
    qDebug()<< "quitting successfully";
}
