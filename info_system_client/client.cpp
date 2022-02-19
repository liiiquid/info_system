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
    user_scroll_list = new scroll_label(this->parent,&users_label);
    user_scroll_list->setFixedSize(250,510);
    user_scroll_list->move(0,60);
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
    while(read_msgs.size() >0)
    {
         res = this->read_msgs.dequeue();
         if(res && !this->islogin)
         {
             if(res->type == 13)
             {
                 this->log_str = res->content;
                 this->parent->update();
             }
             else if(res->type == 12){
                 this->log_str = "登陆成功...";
                 res->print();
                 this->parent->hide_all(this->parent);
                 this->parent->state = 1;
                 this->cur_user = new user(res->sender,"",res->send_type);
                 this->islogin = 1;
                 get_user_info();
                 show_client(this->parent);
             }
             else if(res->type == 99)
             {
                 this->log_str = res->content;
                 this->parent->update();
             }

         }
         else if(res){
             show_data_prepare(res);
         }
         delete res;
    }
}
void client::show_data_prepare(message *mg)
{

    if(mg->type == 4) //talk mail send
    {

    } else if(mg->type == 19)
    {
        get_users_name(mg);
    }
}

void client::get_users_name(message* mg)
{
    clear_users_label(this->users_label);
    clear_users(this->users);
    QVector<QVector<QString>> users_name = message_serialization::analysis_content_2(mg->content);
    if(init_user == 0)
    {
        for(int i = 0; i < users_name.size();i++)
        {
            user* u = new user(message_serialization::str2int(users_name[i][0]),users_name[i][1],1,this->parent,0,i*250+60,250,30);
            QObject::connect(u->label,&head_label::contact_clicked,this,&client::get_contact_info);
            users_label.push_back(u->label);
            users.push_back(u);
        }
    }
    else if(init_user == 1)
    {
        if(cur_user->type == 1)
        {
            for(int i = 0; i < users_name.size();i++)
            {
                user* class_btn = new user(message_serialization::str2int(users_name[i][0]),users_name[i][0]+"班",i+3,this->parent,0,0,250,30);
                QObject::connect(class_btn->label,&head_label::contact_clicked,this,&client::get_contact_info);
                users_label.push_back(class_btn->label);
                users.push_back(class_btn);
            }
        }
        else{
            for(int i = 0; i < users_name.size();i++)
            {
                user* u = new user(message_serialization::str2int(users_name[i][0]),users_name[i][1],2,this->parent,0,i*250+60,250,30);
                QObject::connect(u->label,&head_label::contact_clicked,this,&client::get_contact_info);
                users_label.push_back(u->label);
                users.push_back(u);
            }
        }
        show_contact_list(this->parent);
        user_scroll_list->setAll();
    }
    init_user++;

}

/*slot function used tot receive the click from user class control*/
void client::get_contact_info(int index)
{

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
    QVector<message*>* mgs = new QVector<message*>;
    for(int i = 0; i < sqles.size();i++)
    {
        message* mg = new message(19,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sqles[i]);
        //emit write_to_server(mg);/*write the message to server location*/
        mgs->push_back(mg);
    }
    emit write_to_server_2(mgs);
}

void client::reconnect_server()
{
    this->socket_thread->start();
}


void client::show_client(MainWindow *parent)
{
    this->parent = parent;
    show_main_list(parent);
     cur_page = 1;
    this->title->setText(titles[cur_page]);
    search_label->setText("search...");
    title->show();
    search_label->show();
}

void client::show_main_list(MainWindow *parent)
{
    init_main_list(parent);
    for(int i = 0; i < container_client_main_list.size();i++)
        container_client_main_list[i]->show();
}

void client::show_contact_list(MainWindow* parent)
{
    init_contact_list(parent);
    cur_page = 2;
    hide_all(parent,last_page);
   user_scroll_list->show();
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

    title->move(0,0);
    title->setAlignment(Qt::AlignCenter);
    title->setFixedSize(250,30);
    title->setParent(this->parent);
    search_label->move(0,30);
    search_label->setFixedSize(250,30);
    search_label->setParent(this->parent);
}

void client::init_contact_list(MainWindow* parent) // init the user info display controls and the three controls to control the display content of the scroll list
{
    if(users.size() > 0) return;
    contact_user_info.push_back(head_picture);
    contact_user_info.push_back(name); contact_user_info.push_back(id);contact_user_info.push_back(gender);
    contact_user_info.push_back(age); contact_user_info.push_back(tel);contact_user_info.push_back(_class);

    teacher_opt = new user(0,"teacher_opt",1,this->parent,0,0,250,30);
    users.push_back(teacher_opt);
    users_label.push_back(teacher_opt->label);
    if(cur_user->type == 1)
    {
        class_opt = new user(1,"class_opt",3,this->parent,0,0,250,30);
        users.push_back(class_opt);
        users_label.push_back(class_opt->label);
    }else{
        student_opt = new user(1,"student_opt",2,this->parent,0,0,250,30);
        users.push_back(student_opt);
        users_label.push_back(student_opt->label);
    }
    user_scroll_list->setAll();//used to init the position of these controls in scroll label
    for(int i = 0; i < users_label.size();i++)
    {
        QObject::connect(users_label[i],&head_label::contact_clicked,this,&client::get_contact_info);
    }

    for(int i = 0; i < contact_user_info.size();i++)
    {
        this->contact_user_info[i]->setParent(this->parent);
    }


    head_picture->move(280,30);name->move(410,30);id->move(410,70);
    gender->move(410,110); age->move(490,110);
    _class->move(410,200);tel->move(410,240);

    head_picture->setFixedSize(100,100); name->setFixedSize(100,30);
    id->setFixedSize(100,30); gender->setFixedSize(30,30); age->setFixedSize(30,30);
    _class->setFixedSize(200,30); tel->setFixedSize(200,30);
}

void client::clear_users_label(QVector<head_label *> &label)
{
    int i = 0;
    while(label.size() >0)
    {
        if(label[i]->id < 100) continue;
        delete label[i]; label.removeAt(i);
        i++;
    }
}

void client::clear_users(QVector<user*> &label)
{
    int i = 0;
    while(label.size() >0)
    {
        if(label[i]->id < 100) continue;
        delete label[i]; label.removeAt(i);
        i++;
    }
}
void client::hide_all(MainWindow *parent,int last_page)
{
    switch(last_page)
    {
        case 1: /*消息*/
        {
            search_label->hide();
        }
        break;
        case 2:/*联系人*/
        {
            for(int i  =0; i < contact_user_info.size();i++) contact_user_info[i]->hide();
            user_scroll_list->hide();
            if(communication_btn) communication_btn->hide();
            search_label->hide();
            /*todo communication page hide remain...*/
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
            get_user_info();
            show_contact_list(this->parent);
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
