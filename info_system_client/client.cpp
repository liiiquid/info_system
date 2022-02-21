#include "client.h"
#include "mainwindow.h"
void set_communication_btn(MLabel* communication_btn,client* t,QString id);
extern QMutex mutex_read_queue;
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
          mutex_read_queue.lock();
         res = this->read_msgs.dequeue();
          mutex_read_queue.unlock();
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

    if(mg->type == 4) // communication log
    {
        if(mg->sender == 0 && mg->receiver == 0) {
            qDebug() << mg->content;
            return;
        }
        int id = mg->sender;
        user* u = nullptr;
        for(int i = 0; i < users.size();i++)
        {
            if(users[i]->id == id) {
                u= users[i]; break;
            }
        }
        head_label* info_list_ellm = nullptr;
        if(!(info_list_ellm = info_scroll_list->find_info_elem(id))) /*if info scroll list exist the sender id, which means the page have already established, instead is also as the same*/
        {
            /*haven't talked before*/
            info_list_ellm = new head_label(u,0);
            if(u)
            {
                info_list_ellm->name = u->name;
            }else{
                info_list_ellm->name = "用户x";
            }
            if(info_list_ellm->name == "") info_list_ellm->name = " ";
            info_list_ellm->setText(info_list_ellm->name);
            info_list_ellm->id = id;
            info_scroll_list->add_info_elem(info_list_ellm);
            user_communication_log[id] = new QVector<QLabel*>();
        }
        QLabel* label = new QLabel(mg->content);
        int y = user_communication_log[id]->size() > 0 ? user_communication_log[id]->at(user_communication_log[id]->size() - 1)->y() + 55 : 0;
        label->move(50,y);
        user_communication_log[id]->push_back(label);
        info_list_ellm->unread = 1;
        info_list_ellm->text_assistant = label->text();
        if(user_communication_log[id] == this->info_scroll_page->info_page && !this->info_scroll_page->isHidden())
        {
            this->info_scroll_page->add_page_elem(label);
        }
        this->parent->update();
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
    else if(mg->type ==20)
    {
        get_profile(mg);
    }
    else if(mg->type == 99)
    {
        mg->print();
    }
}

void client::get_profile(message *mg)
{
    /*name, id, gender, age, class, (course), tel*/
    if(cur_user->type == 1)
    {
        QVector<QVector<QString>> results = message_serialization::analysis_content_2(mg->content);
        for(int i = 1; i < all_profile_controls.size();i++)
        {
            if(i == 5)
            {
                QString str = "执教课程 ";
                str += results[0][i];
                str += "                                执教班级 ";
                all_profile_controls[i]->setText(str);
                i++;
                continue;
            }
            all_profile_controls[i]->setText(results[0][i-1]);
        }
        QString str = "";
        for(int i = 0; i < results.size();i++)
        {
             str += results[i][4] + ", ";
        }
        all_profile_controls[5]->setText(all_profile_controls[5]->text() + str);
    }else{
        QVector<QString> results = message_serialization::analysis_content(mg->content);
        for(int i = 1; i < all_profile_controls.size();i++)
        {
            all_profile_controls[i]->setText(results[i-1]);
        }
    }




}

void client::send_profile_info()
{
    QString sql = "";
    if(this->cur_user->type == 1)
    {
        sql = "SELECT teachers.name,teachers.teacher_id,teachers.gender,teachers.age,teacher_class.class_id,courses.course_name,teachers.tel FROM teachers,teacher_class,courses WHERE teachers.course_id = courses.course_id AND ";
             sql += "teachers.teacher_id = teacher_class.teacher_id AND teachers.teacher_id = ";
        sql += message_serialization::int2str(cur_user->id);   sql += ";";
        message* mg = new message(20,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sql);
        emit write_to_server(mg);
    }
    else{
        sql = "SELECT stu_name,stu_id,gender,age,class_id,tel FROM students WHERE stu_id="; sql += cur_user->id; sql+=";";
        message* mg = new message(20,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sql);
        emit write_to_server(mg);
    }

}

void client::change_profile()
{
    QString sql = "";
    if(cur_user->type == 1)
    {
        QString attributes[7] = {"","teacher_name","","gender","age","","tel"};
        sql = "UPDATE teachers SET ";
        for(int i = 1; i < all_profile_controls.size();i++)
        {
            if(all_profile_controls[i]->edited)
            {
                if(i == 1 || i == 3|| i == 6)
                {
                    sql += attributes[i]; sql += "='";
                    if(i == 6)
                    {
                        QString str = all_profile_controls[i]->text();
                        sql += message_serialization::split(str,":")[1];
                    }else sql += all_profile_controls[i]->text();
                    sql +="',";
                }else{
                    sql += attributes[i]; sql += "="; sql += all_profile_controls[i]->text(); sql +=",";
                }
            }
        }
        sql[sql.length()-1] = ' ';
        sql+= "WHERE teacher_id = "; sql += message_serialization::int2str(cur_user->id); sql += ";";
    }else{
        QString attributes[7] = {"","stu_name","","gender","age","","tel"};
        sql = "UPDATE students SET ";
        for(int i = 1; i < all_profile_controls.size();i++)
        {
            if(all_profile_controls[i]->edited)
            {
                if(i == 1 || i == 3|| i == 6)
                {
                    sql += attributes[i]; sql += "='";
                    sql += all_profile_controls[i]->text(); sql +="',";
                }else{
                    sql += attributes[i]; sql += "="; sql += all_profile_controls[i]->text(); sql +=",";
                }
            }
        }
        sql[sql.length()-1] = ' ';
        sql+= "WHERE stu_id = "; sql += message_serialization::int2str(cur_user->id); sql += ";";
    }
    qDebug() << "-------------------------------------------";
    qDebug() << sql;
    emit write_to_server(new message(11,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sql));
}

void client::get_users_name(message* mg) // refresh the user friends information each time click the contact btn, temporarily can not realize that
{
    QVector<QVector<QString>> users_name = message_serialization::analysis_content_2(mg->content);
    if(init_user == 0)
    {
            clear_users(&users);
            user_scroll_list->clear_scroll_list();
            head_label* teacher_opt = new head_label(nullptr);
            teacher_opt->setText("老师");
            user_scroll_list->add_first_control(teacher_opt);
            if(cur_user->type == 2)
            {
                head_label* student_opt = new head_label(nullptr);
                student_opt->setText("同学");
                user_scroll_list->add_first_control(student_opt);
            }
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
                get_contact_info(class_btn);
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
    if(init_user == 2) init_user = 0;
}

void client::get_class_student(message *mg)
{
    mg->print();
    if(mg->content == '0') return;
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
        emit write_to_server(mg);
    }
    else
    {
        /*clicked teacher tab*/
        if(ellm->parent->type == 1)
        {
            QString sql = "SELECT teachers.name,teachers.teacher_id,teachers.gender,teachers.age,teacher_class.class_id,teachers.tel FROM teachers,teacher_class WHERE teacher_class.teacher_id= teachers.teacher_id and teachers.teacher_id = "; sql +=  message_serialization::int2str(ellm->id); sql+= ";";
            message* mg = new message(10,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sql);
            emit write_to_server(mg);
        }else{
            QString sql = "SELECT stu_name,stu_id,gender,age,class_id,tel FROM students WHERE stu_id = "; sql +=  message_serialization::int2str(ellm->id); sql+= ";";
            message* mg = new message(10,cur_user->id,cur_user->type,cur_user->id,cur_user->type,sql);
            emit write_to_server(mg);
        }
    }
}

/*ask the server send the user data of this client*/
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
        mgs->push_back(mg);
    }
    emit write_to_server_2(mgs);
}

void client::send_info_friends()
{
    QLabel* page_log_elem = new QLabel( this->text_input->toPlainText());
    int y = info_scroll_page->info_page->size() >0  ? info_scroll_page->info_page->last()->y() + 55 : 0;
    page_log_elem->move(290,y);
    this->info_scroll_page->add_page_elem(page_log_elem);
    this->text_input->setPlainText("");
    this->info_scroll_list->find_info_elem(message_serialization::str2int(this->info_send_btn->id))->text_assistant = page_log_elem->text();
    message* mg = new message(4,cur_user->id,cur_user->type,message_serialization::str2int(this->info_send_btn->id),this->info_send_btn->type,page_log_elem->text());
    emit write_to_server(mg);
}

void client::reconnect_server()
{
    this->socket_thread->start();
}

void client::show_client(MainWindow *parent)
{
    this->parent = parent;
     init_contact_list(this->parent);
    get_user_info();
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
     if(name_title->text()  != "")
     {
         name_title->show();
         info_scroll_page->show();
         text_input->show();
         info_send_btn->show();
         back->show();
     }

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

void client::show_profile_list(MainWindow *parent)
{
    init_profile_list(parent);
    cur_page = 4;
    hide_all(parent,last_page);
    title->setText(titles[cur_page-1]);
    send_profile_info();
    for(int i  =0; i < all_profile_controls.size();i++)
    {
        all_profile_controls[i]->show();
    }
    update_profile->show();
    profile_btn->show();
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
    info_scroll_list->is_info = 1;
    info_scroll_page = new scroll_label(this,this->parent);
    info_scroll_page->is_info = 2;

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
    QObject::connect(info_send_btn,&MLabel::label_clicked,this,&client::main_process);
    QObject::connect(back,&MLabel::label_clicked,this,&client::main_process);

}

void client::init_profile_list(MainWindow *parent)
{
    if(all_profile_controls.size() > 0) return;
    all_profile_controls.push_back(picture);
    all_profile_controls.push_back(profile_name); all_profile_controls.push_back(profile_id);all_profile_controls.push_back(profile_gender);
    all_profile_controls.push_back(profile_age);all_profile_controls.push_back(profile_class); all_profile_controls.push_back(profile_tel);
    profile_class->can_edit = false;
    for(int i = 0; i < all_profile_controls.size();i++)
    {
        this->all_profile_controls[i]->setParent(this->parent);
        QPalette sample_palette;
        sample_palette.setColor(QPalette::Window, Qt::white);
        sample_palette.setColor(QPalette::WindowText, Qt::blue);
        all_profile_controls[i]->setAutoFillBackground(true);
        all_profile_controls[i]->setPalette(sample_palette);
    }
    profile_name->setText("姓名"); if(cur_user->type == 1)profile_id->setText("工号"); else profile_id->setText("学号");
    profile_gender->setText("性别");profile_age->setText("年龄");if(cur_user->type==1) profile_class->setText("执教班级"); else profile_class->setText("班级"); profile_tel->setText("电话:");
    picture->move(280,30);profile_name->move(410,30);profile_id->move(410,70);
    profile_gender->move(410,110); profile_age->move(490,110);
    profile_class->move(270,200);profile_tel->move(270,240);

    picture->setFixedSize(100,100); profile_name->setFixedSize(100,30);
    profile_id->setFixedSize(100,30); profile_gender->setFixedSize(30,30); profile_age->setFixedSize(30,30);
    profile_class->setFixedSize(500,30); profile_tel->setFixedSize(500,30);
    update_profile->setFixedSize(550,50);
   update_profile->setParent(this->parent);
   QObject::connect(update_profile,&MLabel::label_clicked,this,&client::main_process);
    update_profile->move(253,300);
    profile_id->can_edit = false;
    profile_btn->setFixedSize(245,50);
   profile_btn->setParent(this->parent);
   QObject::connect(profile_btn,&MLabel::label_clicked,this,&client::main_process);
    profile_btn->move(0,30);
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
            for(int i = 0; i < all_profile_controls.size();i++)
            {
                all_profile_controls[i]->hide();
            }
            this->update_profile->hide();
            this->profile_btn->hide();
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
            show_profile_list(this->parent);
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
            send_info_friends();
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
        case 8:/*修改个人信息*/
        {
            change_profile();
        }
        break;
        case 9:/*显示修改个人信息*/
        {
            this->cur_page = 4;
            show_profile_list(this->parent);
        }
        break;

    }
}

head_label* client::check_send_page_open(int id)
{

    return this->info_scroll_list->find_info_elem(id);
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
    head_label* label = check_send_page_open(id);
     if(label)
    {
         info_scroll_page->set_scroll_page(user_communication_log[id]);
         name_title->setText(u->name);
         int last_pos  = user_communication_log[u->id]->size()-1;
         if(last_pos > 0)
         label->text_assistant = user_communication_log[u->id]->at(last_pos)->text();
         show_info_list(this->parent);
         return;
     }else{
         qDebug() << message_serialization::int2str(id)<<"elem not exist in list... client::open_page(int id)";
     }

    head_label* info_list_elem = new head_label(u,0);
    info_list_elem->name = u->name;
    if(info_list_elem->name == "") info_list_elem->name = " ";
    this->name_title->setText(info_list_elem->name);
    info_list_elem->setText(u->name);
    info_list_elem->id = id;
    info_scroll_list->add_info_elem(info_list_elem);
    if(user_communication_log.find(u->id) == user_communication_log.end()) {
        QVector<QLabel*>* log = new QVector<QLabel*>();
        user_communication_log.insert(u->id,log);
        info_list_elem->text_assistant = "";
    }else{
        int last_pos  = user_communication_log[u->id]->size()-1;
        if(last_pos > 0)
        info_list_elem->text_assistant = user_communication_log[u->id]->at(last_pos)->text();
    }
    info_scroll_page->set_scroll_page(user_communication_log[u->id]);
    info_send_btn->id = message_serialization::int2str(id);
    info_send_btn->type = u->type;
    show_info_list(this->parent);
}

void client::set_send_page(head_label* ellm)
{
    qDebug() << message_serialization::int2str(ellm->id);
    this->name_title->setText(ellm->name);
    info_scroll_page->set_scroll_page(user_communication_log[ellm->id]);
    info_send_btn->id = message_serialization::int2str(ellm->id);
    info_send_btn->type = ellm->parent->type;
    show_info_list(this->parent);
}
