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
            QObject::connect(this,&mthread::user_exit,this->socket,&t_socket::close_self);
            QObject::connect(this->client_ptr,SIGNAL(write_to_server_2(QVector<message*>*)),this->socket,SLOT(write_out_2(QVector<message*>*)));
            this->client_ptr->isconnected= 1;
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
    QVector<QString> mgs_str = message_serialization::analysis_serialize_2(shared);
    for(int i = 0; i < mgs_str.size();i++)
    {
        message* tm = message_serialization::unserialize(mgs_str[i]);
         this->client_ptr->read_msgs.enqueue(tm);
    }
    emit process_ok();
}

void mthread::process()
{

}

void mthread::close_thread()
{
    this->quit();
    m_event* ev = new m_event(QEvent::Type(QEvent::User+1));
    QCoreApplication::postEvent(this->socket,ev); /*used to notify the t_socket to exit safely*/
    this->terminate();
    qDebug() << "programe safely exit in mthread...";
}

void mthread::quit_thread()
{
    this->quit();
    socket->close();
    delete this->socket;
    this->client_ptr->isconnected= 0;
    this->client_ptr->read_msgs.push_back(new message(99,0,0,0,0,QString("服务器连接失败...")));
    emit connect_failed();/*used to tell the main window connection failed...*/
    this->terminate();
    emit reconnect();/*used to tell the main window start the connect thread again to connect with the server... */
    qDebug()<< "quitting successfully";
}
