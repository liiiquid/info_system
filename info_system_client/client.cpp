#include "client.h"
#include "mainwindow.h"
void set_communication_btn(MLabel* communication_btn,client* t,QString id);

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
                 this->user_scroll_list= new scroll_label(this,this->parent);
                 this->user_scroll_list->setFixedSize(250,510);
                 this->user_scroll_list->move(0,60);
                 QObject::connect(this,&client::update_user_scroll_list,this->user_scroll_list,&scroll_label::update_list);

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

    }else if(mg->type == 10)//teacher information
    {
        get_user_profile(mg);
    }
    else if(mg->type == 16)//class student information
    {
        get_class_student(mg);
    }
    else if(mg->type == 19) // init class, teacher, student information
    {
        get_users_name(mg);
    }
}

void client::get_users_name(message* mg) // refresh the user friends information each time click the contact btn, temporarily can not realize that
{
    QVector<QVector<QString>> users_name = message_serialization::analysis_content_2(mg->content);
    if(init_user == 0)
    {
        clear_users(&users);
        user_scroll_list->clear_scroll_list();
        for(int i = 0; i < users_name.size();i++)
        {
            user* u = new user(message_serialization::str2int(users_name[i][0]),users_name[i][1],1,this->parent,0,i*250+60,250,30);
 /*************************************************************the num represent the level in the scroll list, below are as same mean******************************************************************************************************************/
            user_scroll_list->add_second_control(user_scroll_list->fir_level_controls[0],u->label); /*add teachers*/
            users.push_back(u);
        }
    }
    else if(init_user == 1)
    {
        if(cur_user->type == 1)
        {
            for(int i = 0; i < users_name.size();i++) /*get the teacher's teach classes/*/
            {
                head_label* class_btn = new head_label(nullptr);
                class_btn->setText(users_name[i][0] + "班");
                class_btn->is_get_server = 1;
                class_btn->id = message_serialization::str2int(users_name[i][0]);
                user_scroll_list->add_first_control(class_btn); /*add btns*/
            }
        }
        else{
            for(int i = 0; i < users_name.size();i++)/*get the student's classmates...*/
            {
                user* u = new user(message_serialization::str2int(users_name[i][0]),users_name[i][1],2,this->parent,0,i*250+60,250,30);
                user_scroll_list->add_second_control(user_scroll_list->fir_level_controls[1],u->label); /*add students*/
                users.push_back(u);
            }
        }
    }
    init_user++;
}

void client::get_class_student(message *mg)
{
    if(mg->content == ';') return;
    QVector<QVector<QString>> users_name = message_serialization::analysis_content_2(mg->content);

    head_label* target_main_control = nullptr;
    for(int i = 0; i < user_scroll_list->fir_level_controls.size();i++)
    {
        if(user_scroll_list->fir_level_controls[i]->id == message_serialization::str2int(users_name[i][2]))
        {
            target_main_control = user_scroll_list->fir_level_controls[i]; break;
        }
    }

    for(int i = 0; i < users_name.size();i++)
    {
        user* u = new user(message_serialization::str2int(users_name[i][0]),users_name[i][1],2,this->parent,0,i*250+60,250,30);
        user_scroll_list->add_second_control(target_main_control,u->label); /*add students*/
        users.push_back(u);
    }
    target_main_control->is_get_server = 0;// temporarily reject get data secondly..
    emit update_user_scroll_list(target_main_control);
}


/*process the friends' profile page*/
void client::get_user_profile(message *mg)
{

    QVector<QVector<QString>> user_name_profile = message_serialization::analysis_content_2(mg->content);
     communication_btn->setId(user_name_profile[0][1]);
     if(user_name_profile.size() == 1)contact_user_profile[5]->setText("班级"); else contact_user_profile[5]->setText("执教班级");
    for(int i =1; i < this->contact_user_profile.size();i++)
    {
        if(i == 5) continue;
        if(user_name_profile[0][i-1] == "" || user_name_profile[0][i-1]=="0")  {contact_user_profile[i]->setText("无"); continue;}
        contact_user_profile[i]->setText(user_name_profile[0][i-1]);
    }
    for(int i  = 0; i < user_name_profile.size();i++)
    {
        if(user_name_profile[i][4] == "0") continue;
        else contact_user_profile[5]->setText(contact_user_profile[5]->text() + ", " + user_name_profile[i][4]);
    }
    for(int i = 0; i < contact_user_profile.size();i++)
    {
        contact_user_profile[i]->show();
    }
    communication_btn->show();
}

/*slot function used tot receive the click from user class control*/
void client::get_contact_info(head_label* ellm)
{

    if(ellm->id < 10000)
    {
        QString sqles = "SELECT stu_id,stu_name,class_id FROM students where class_id = "; sqles+= message_serialization::int2str(ellm->id); sqles+=";";
        message* mg = new message(16,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sqles);
        write_to_server(mg);
    }
    else
    {
        /*clicked teacher tab*/
        if(ellm->parent->type == 1)
        {
            QString sql = "SELECT teachers.name,teachers.teacher_id,teachers.gender,teachers.age,teacher_class.class_id,teachers.tel FROM teachers,teacher_class WHERE teacher_class.teacher_id= teachers.teacher_id and teachers.teacher_id = "; sql +=  message_serialization::int2str(ellm->id); sql+= ";";
            message* mg = new message(10,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sql);
            write_to_server(mg);
        }else{
            QString sql = "SELECT stu_name,stu_id,gender,age,class_id,tel FROM students WHERE stu_id = "; sql +=  message_serialization::int2str(ellm->id); sql+= ";";
            message* mg = new message(10,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sql);
            write_to_server(mg);
        }

    }
}

/*ask the server send the user data of this client*/
void client::get_user_info()
{
    if(users.size() > 0) return;
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
     show_info_list(parent);

}

void client::show_main_list(MainWindow *parent)
{
    init_main_list(parent);
    for(int i = 0; i < container_client_main_list.size();i++)
        container_client_main_list[i]->show();
}

void client::show_info_list(MainWindow *parent)
{
    init_info_list(parent);
    cur_page = 1;
    hide_all(parent,last_page);
    search_label->setText("search...");
    search_label->show();
    title->setText(titles[cur_page-1]);
    title->show();
     info_scroll_list->show();
}

void client::show_contact_list(MainWindow* parent)
{

    init_contact_list(parent);
    cur_page = 2;
    hide_all(parent,last_page);
    search_label->setText("search...");
   search_label->show();
   title->setText(titles[cur_page-1]);
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
    if(contact_user_profile.size() > 0) return;
    contact_user_profile.push_back(head_picture);
    contact_user_profile.push_back(name); contact_user_profile.push_back(id);contact_user_profile.push_back(gender);
    contact_user_profile.push_back(age);contact_user_profile.push_back(_class); contact_user_profile.push_back(tel);

    for(int i = 0; i < contact_user_profile.size();i++)
    {
        this->contact_user_profile[i]->setParent(this->parent);
        QPalette sample_palette;
        sample_palette.setColor(QPalette::Window, Qt::white);
        sample_palette.setColor(QPalette::WindowText, Qt::blue);

        contact_user_profile[i]->setAutoFillBackground(true);
        contact_user_profile[i]->setPalette(sample_palette);
    }

    head_picture->move(280,30);name->move(410,30);id->move(410,70);
    gender->move(410,110); age->move(490,110);
    _class->move(270,200);tel->move(270,240);

    head_picture->setFixedSize(100,100); name->setFixedSize(100,30);
    id->setFixedSize(100,30); gender->setFixedSize(30,30); age->setFixedSize(30,30);
    _class->setFixedSize(200,30); tel->setFixedSize(200,30);
    head_label* teacher_opt = new head_label(nullptr);
    teacher_opt->setText("老师");
    user_scroll_list->add_first_control(teacher_opt);
    if(cur_user->type == 2)
    {
        head_label* student_opt = new head_label(nullptr);
        student_opt->setText("同学");
        user_scroll_list->add_first_control(student_opt);
    }

}

void client::init_info_list(MainWindow* parent) // init the page to receive the information which friends send
{
    if(text_input) return;
    text_input = new QTextEdit();
    info_scroll_list = new scroll_label(this,this->parent);
    info_scroll_page = new scroll_label(this,this->parent);

    text_input->setParent(this->parent);
    info_send_btn->setParent(this->parent);
    info_scroll_list->setParent(this->parent);
    info_scroll_page->setParent(this->parent);
    name_title->setParent(this->parent);

    back->setParent(this->parent);

    info_scroll_list->setFixedSize(250,510);
    info_scroll_page->setFixedSize(550,370); name_title->setFixedSize(100,30);back->setFixedSize(100,30);
    text_input->setFixedSize(550,200);info_send_btn->setFixedSize(100,30);

    info_scroll_list->move(0,60);
    info_scroll_page->move(250,30);back->move(250,0);name_title->move(350,0);
    text_input->move(250,400);info_send_btn->move(600,570);

    communication_btn->setFixedSize(550,50);
   communication_btn->setParent(this->parent);
   QObject::connect(communication_btn,&MLabel::label_clicked,this,&client::main_process);
    communication_btn->move(250,300);

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

void client::clear_users(QVector<user*>* label)
{
    while(label->size() > 0)
    {
        delete label->last(); label->removeLast();
    }
}

void client::hide_all(MainWindow *parent,int last_page)
{
    switch(last_page)
    {
        case 1: /*消息*/
        {
            search_label->hide();
            info_scroll_list->hide();
            info_scroll_page->hide();
            name_title->hide();
            back->hide();
            info_send_btn->hide();
            text_input->hide();
        }
        break;
        case 2:/*联系人*/
        {
            for(int i  =0; i < contact_user_profile.size();i++) contact_user_profile[i]->hide();
            if(communication_btn) communication_btn->hide();
            search_label->hide();
            user_scroll_list->hide();
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

    switch(index)
    {
        case 0:
            delete cur_user;
            this->socket_thread->close_thread();
            QApplication::exit();
            break;
        case 1: /*消息*/
        {
            this->cur_page = index;

            show_info_list(this->parent);
        }
            break;
        case 2:/*联系人*/
        {
            this->cur_page = index;
            get_user_info();
            show_contact_list(this->parent);
        }
            break;
        case 3:/*成绩*/
        {
            this->cur_page = index;
        }
            break;
        case 4:/*我*/
        {
            this->cur_page = index;
        }
            break;
        case 5:/*发送消息按钮*/
        {
            if(cur_page == 2)
                open_send_page(message_serialization::str2int(communication_btn->id));
        }
        break;
        case 6:/*发送消息给服务器*/
        {

        }
        break;
        case 7:/*返回上一页*/
        {
            this->name_title->hide();
            this->back->hide();
            this->text_input->hide();
            this->info_scroll_page->hide();
            this->info_send_btn->hide();
        }
        break;

    }
}

void client::open_send_page(int id)/*from contact page to info page, and add a contact user to info list*/
{
    user* u = nullptr;
    for(int i = 0; i < users.size();i++)
    {

        if(users[i]->id == id)
        {
            u = users[i]; break;
        }
    }
    head_label* info_list_elem = new head_label(u,0);
    this->name_title->setText(u->name);
    info_list_elem->setText(u->name);
    info_scroll_list->add_info_elem(info_list_elem);
    if(user_communication_log.find(u->id) == user_communication_log.end()) {
        QVector<QLabel*>* log = new QVector<QLabel*>();
        user_communication_log.insert(u->id,log);
        info_list_elem->text_assistant = "";
    }else{
        info_list_elem->text_assistant = user_communication_log[u->id]->at(user_communication_log[u->id]->size()-1)->text();
    }
    info_scroll_page->set_scroll_page(user_communication_log[u->id]);

    info_scroll_page->show();
    name_title->show();
    back->show();
    info_send_btn->show();
    text_input->show();
    show_info_list(this->parent);
}

void client::set_send_page(head_label* ellm)
{
    this->name_title->setText(ellm->parent->name);
    info_scroll_page->set_scroll_page(user_communication_log[ellm->id]);
    show_info_list(this->parent);
}
