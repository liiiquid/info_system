#include "client.h"
#include "mainwindow.h"
client::client()
{
    mutex_read = new QMutex();
    socket_thread = new mthread(this);
    QObject::connect(socket_thread,&mthread::connect_failed, this,&client::show_data);
    QObject::connect(socket_thread,&mthread::connect_success,this,&client::show_data);
    socket_thread->start();
    this->socket=socket_thread->socket;
    qDebug() << socket_thread->isRunning();
    this->log_str = "欢迎使用学生管理系统...";
}

client::~client()
{
    if(socket_thread)
    {
        socket_thread->terminate();
        delete socket_thread;
    }
}


bool client::client_login(message msg)
{
    if(!this->isconnected) {this->log_str = "请先开启服务器..."; return false;}
    QVector<QString> information = message_serialization::analysis_content(msg.content);
    QString sql_login;
    int sender = message_serialization::str2int(information[0]);
    if(msg.send_type == 1)
    {
        sql_login = "SELECT teachers.teacher_id, users.password FROM teachers, users WHERE users.user_id =  "; sql_login +=information[0];
        sql_login += " and users.password ='"; sql_login+=  information[1]; sql_login+= "' and teachers.teacher_id = users.user_id;";
    }
    else{
        sql_login = "SELECT students.stu_id, users.password FROM students, users WHERE users.user_id =  "; sql_login +=information[0];
        sql_login += " and users.password ='"; sql_login+=  information[1]; sql_login+= "' and students.stu_id = users.user_id;";
    }
    message* m = new message(msg.type,sender,msg.send_type,0,0,sql_login);
    if(this->isconnected)emit write_to_server(m);
    this->parent->isload = 1;
    this->log_str = "正在登陆中,请稍后...";
    /*login... wait for the response of socket...*/
    return false;
}

bool client::client_register(message msg)
{
    if(!this->isconnected) {this->log_str = "请先开启服务器..."; return false;}
    QVector<QString> information = message_serialization::analysis_content(msg.content);
    QString sql_register;
    int sender = message_serialization::str2int(information[0]);
    sql_register = "INSERT INTO users values( "; sql_register +=information[0]; sql_register += ",'";
    sql_register+=  information[1]; sql_register+= "');";
    message* m = new message(msg.type,sender,msg.send_type,0,0,sql_register);
    if(this->isconnected)emit write_to_server(m);
    this->parent->isload = 1;
    this->log_str = "管理员同意后即可登陆账号...";
    /*login... wait for the response of socket...*/
    return false;
}

/*slot function which process the data from server*/
void client::show_data()
{
    message* res = nullptr;
    if(this->read_msgs.size() > 0) res = this->read_msgs.dequeue();
    if(res)
    {
        if(res->type == 13)
        {
            this->log_str = res->content;
            this->parent->update();
        }
        else if(res->type == 12){

            this->parent->hide_all(this->parent);
            this->parent->state = 1;
            show_client(this->parent);
        }
        else if(res->type == 99)
        {
            this->log_str = res->content;
            this->parent->update();
        }
        delete res;
    }

}


void client::show_client(MainWindow *parent)
{
    this->parent = parent;
    init_main_list(parent);
}

void client::show_main_list(MainWindow *parent)
{

}

void client::init_main_list(MainWindow* parent)
{

}

