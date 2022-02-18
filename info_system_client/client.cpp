#include "client.h"
#include "mainwindow.h"
client::client()
{
    mutex_read = new QMutex();
    socket_thread = new mthread(this);
    QObject::connect(socket_thread,&mthread::connect_failed, this,&client::show_data);
    QObject::connect(socket_thread,&mthread::connect_success,this,&client::show_data);
    QObject::connect(socket_thread,&mthread::reconnect,this,&client::reconnect_server);
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

void client::show_data_prepare(message *mg)
{
    if(mg->type == 4) //talk mail send
    {

    }
    delete mg;
}


/*slot function which process the data from server*/
void client::show_data()
{
    message* res = nullptr;
    if(read_msgs.size() >0 ) res = this->read_msgs.dequeue();
    if(res && !this->islogin)
    {
        if(res->type == 13)
        {
            this->log_str = res->content;
            this->parent->update();
        }
        else if(res->type == 12){
            this->log_str = "登陆成功...";
            this->parent->hide_all(this->parent);
            this->parent->state = 1;
            this->cur_user = new user(res->sender,"",res->send_type);
            get_user_info();
            //show_client(this->parent);

        }
        else if(res->type == 19)
        {
            res->print();
            res->print();
        }
        else if(res->type == 99)
        {
            this->log_str = res->content;
            this->parent->update();
        }
        delete res;
    }
    else if(res){
        show_data_prepare(res);
    }
}

void client::get_user_info()
{
    QVector<QString> sqles;
    QString sql = "SELECT teacher_id, name FROM teachers WHERE teacher_id <> "; sql += message_serialization::int2str(cur_user->id);
    sql += ";"; sqles.push_back(sql);
    if(cur_user->type == 1) /*teacher*/
    {
        sql = "SELECT class_id FROM teacher_class WHERE teacher_id = "; sql += message_serialization::int2str(cur_user->id); sql += ";";
        sqles.push_back(sql);
    }
    else{
        sql = "SELECT stu_id,stu_name FROM students WHERE class_id = (SELECT class_id FROM students WHERE stu_id = "; sql +=  message_serialization::int2str(cur_user->id); sql += ");";
        sqles.push_back(sql);
    }

    for(int i = 0; i < sqles.size();i++)
    {
        message* mg = new message(19,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sqles[i]);
        emit write_to_server(mg);/*write the message to server location*/
    }
}


void client::reconnect_server()
{
    this->socket_thread->start();
}


void client::show_client(MainWindow *parent)
{
    this->parent = parent;
    show_main_list(parent);
}

void client::show_main_list(MainWindow *parent)
{
    init_main_list(parent);
    for(int i = 0; i < container_client_main_list.size();i++)
        container_client_main_list[i]->show();
}

void client::init_main_list(MainWindow* parent)
{
    if(container_client_main_list.size() >0) return;

    container_client_main_list.push_back(communication);
    container_client_main_list.push_back(contacts);
    container_client_main_list.push_back(grade);
    container_client_main_list.push_back(me);
    container_client_main_list.push_back(exit);

    for(int i  = 0; i < container_client_main_list.size();i++)
    {
        container_client_main_list[i]->setParent(this->parent);
        container_client_main_list[i]->setFixedSize(50,30);
        container_client_main_list[i]->move(i*50,569); // previously remain three point to completely show the btn;
        QObject::connect(container_client_main_list[i],&MLabel::label_clicked,this,&client::main_process);
    }
}

void client::hide_all(MainWindow *parent,int last_page)
{
    switch(last_page)
    {
        case 1: /*消息*/
        {

        }
        break;
        case 2:/*联系人*/
        {

        }
        break;
        case 3:/*成绩*/
        {

        }
        break;
        case 4:/*我*/
        {

        }
        break;
    }
}

void client::main_process(int index)
{

    this->last_page = this->cur_page;
    this->cur_page = index;
    this->hide_all(parent,last_page);
    switch(index)
    {
        case 0:
            delete cur_user;
            this->socket_thread->close_thread();
            QApplication::exit();
            break;
        case 1: /*消息*/
        {

        }
            break;
        case 2:/*联系人*/
        {

        }
            break;
        case 3:/*成绩*/
        {

        }
            break;
        case 4:/*我*/
        {

        }
            break;
    }
}
