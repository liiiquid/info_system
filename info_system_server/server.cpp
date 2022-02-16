#include "server.h"

server::server()
{
 qDebug() << "thread server:: " << QThread::currentThread() << ": is running...";
    this->dbo = new db_operation("QMYSQL","localhost","stu_mgr","root","root");
    this->socket_server = new TServer(this);
    QObject::connect(this->socket_server,SIGNAL(show_new_connection(int)),this,SLOT(print_server_info_log(int)));

/*************************************************************当时只道是寻常 2022 2.11****************************************************************************************************/
/*-------------------------------------------------------------------------ui object initialization below is -----------------------------------------------------*/
    log_message = new QVector<message*>;
}

server::~server()
{
    if(this->dbo)delete this->dbo;
    if(this->socket_server) delete this->socket_server;
    /*make sure all the old msg are remove*/
    for(int i = 0; i < msg_container_list.size();i++)
    {if(msg_container_list[0]) delete msg_container_list[0]; msg_container_list.removeFirst();}
    release_ui_vector(all_info_controls);
    release_ui_vector(all_msg_controls);
    release_ui_vector(all_server_controls);
    release_ui_vector2(add_container_list);
    release_ui_vector(all_add_controls);
    release_ui_vector(msg_container_list);
    for(int i = 0; i < this->admins.size();i++)
    {
        if(this->admins.first())
            delete this->admins.first();
        this->admins.pop_back();
    }

}

bool server::admin_login(message msg)
{
    QVector<QString> up = analysis_message(msg);
    QString sql = "SELECT admin_id,password from administrator where admin_id = ";
    sql+=up[0];
    sql+=" and password = '";
    sql += up[1];
    sql += "';";
    QString result = dbo->select(sql);
    QVector<QString> ress;
    ress= msg_serial.split(result,"\t\n");

    return ress[ress.size()-1].toInt() > 0;
}
bool server::admin_register(message msg)
{
    QVector<QString> up = analysis_message(msg);
    QString sql = "INSERT INTO administrator(admin_id,password) VALUES(";
    sql+=up[0];
    sql += ", '";
    sql+= up[1];
    sql += "');";
    qDebug() <<sql;
    int result = dbo->update(sql);
    return result > 0;
}

/*default paramters*/
bool server::server_start()
{
    if(this->socket_server->isListening()) return false;
    QHostAddress addr("127.0.0.1");
    this->socket_server->setMaxPendingConnections(30);
    this->socket_server->listen(addr,12345);
    return true;
}

bool server::server_end()
{
    /*for(int i = 0; i < this->socket_server->socket_clients.size();i++)
        if(this->socket_server->socket_clients[i]) delete this->socket_server->socket_clients[i];*/
    this->socket_server->close();
    return true;
}

void server::show_server(QWidget* parent)
{
    init_main();
    this->show_main_list(parent);
}

void server::show_main_list(QWidget *parent)
{
    for(int i = 0; i < labels_main_list;i++)
    {
         this->labels[i]->show();
    }
}

void server::show_ser_list(QWidget *parent)
{
    init_server();
    if(check_server_open()) return;
    this->server_info_log->show();
    this->parent->update();
    for(int i = 0; i < labels_server_control;i++)
    {
        this->server_control[i]->show();
    }
    is_server_open = 1;
}


/*demo method*/
/*get all the information which need to display in the msg list, and it will clear all the elements in msg_account member, and then fill it again*/
void server::get_msg_list()
{
    int pos = 0;
    clear_container_list(msg_container_list);
    for(int i = this->curend_msg; i < this->socket_server->unproced_messages.size() && i < this->curend_msg + MSG_LIST_NUM;i++)
    {
        MLabel* label = new MLabel(IDENTIFIER_MSG,pos);
        QObject::connect(label,SIGNAL(msg_clicked(int)),this,SLOT(print_msg_page(int)));
        label->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
        label->move(MSG_LIST_LABEL_X,pos*MAIN_LABEL_HEIGHT+20);
        QString cont = msg_serial.int2str(this->socket_server->unproced_messages.at(i)->sender);
        /*indentify the msg*/
        cont += "  ";
        cont += message::type_table[this->socket_server->unproced_messages.at(i)->type];
        label->setText(cont);
        label->setParent(this->parent);
        this->msg_container_list.push_back(label);pos++;
    }/*for*/
}

void server::show_msg_list(QWidget *parent)
{
    init_msg();
    if(check_msg_open()) return;
    get_msg_list();
    for(int i = 0;i<this->msg_container_list.size();i++)
        this->msg_container_list[i]->show();
    this->next->show();
    this->last->show();
    is_msg_open = 1;
}
void server::update_msg_list() /************slot function, this function is in order to update the message list************************************************/
{
    if(ui_page == 1)
    {
        this->hide_all(parent,1);
       switch_msg_list(this->parent);
    }
}

void server::switch_msg_list(QWidget *parent)
{
    get_msg_list();
    for(int i = 0;i<this->msg_container_list.size();i++)
        this->msg_container_list[i]->show();
    this->next->show();
    this->last->show();
    if(this->is_msg_page_open){
        this->unproced_msg_content_main->show();
        this->unproced_msg_content_top->show();
        this->msg_ok->show();
        this->msg_no->show();
    }
    is_msg_open = 1;
}
void server::agree_msg()
{
    QVector<QString> sqles = message_serialization::analysis_content(cur_msg->content);
    for(int i = 0; i < sqles.size();i++)
    {
        if(this->dbo->update(sqles[i]) >= 0)
        {
            qDebug() << "changed db successfully...";
        }
    }
    for(int i = 0; i < this->socket_server->unproced_messages.size();i++)
    {
        if(this->cur_msg == socket_server->unproced_messages[i]) {
            delete cur_msg;
            socket_server->unproced_messages.removeAt(i);
            this->unproced_msg_content_main->setText("");
            switch_msg_list(this->parent);
            return;}/*refresh the msg list again*/
    }/*for*/
}

void server::disagree_msg()
{
    for(int i = 0; i < this->socket_server->unproced_messages.size();i++)
    {
        if(this->cur_msg == socket_server->unproced_messages[i]) {
            delete cur_msg;
            qDebug() << "remove msg successfully...";
            socket_server->unproced_messages.removeAt(i);
            this->unproced_msg_content_main->setText("");
            switch_msg_list(this->parent);
            return;
        }
    }/*for*/

}

/*core function of online numer display*/
void server::show_online_num(QWidget *parent)
{
    qDebug()<<"notification: current online people number is "<<this->socket_server->socket_clients.size();
}

void server::show_info_main(QWidget *parent)
{
    init_info();
    for(int i = 0; i < labels_select_control;i++)
        this->info_control_list[i]->show();
}

void server::get_info_list()
{
    clear_container_list(this->info_container_list);/*--------------------------------------clear all the info container list to get a new container list of info element-------------------*/
    QString sql;
    if(this->isteacher)
        sql = "SELECT teacher_id FROM teachers LIMIT ";
    else sql = "SELECT stu_id FROM students LIMIT ";
    sql += message_serialization::int2str(this->curend_info);
    sql += ",";
    sql += message_serialization::int2str(INFO_LIST_NUM);
    sql += ";";
    QVector<QString> reses = message_serialization::analysis_content(this->dbo->select(sql));

    for(int i = 0; i < reses.size();i++)
    {
        MLabel* info_element = new MLabel(IDENTIFIER_INFO,i,reses[i]);
        info_element->id = reses[i];
        QObject::connect(info_element,SIGNAL(info_clicked(int)),this,SLOT(print_info_page(int)));
        info_element->move(INFO_LIST_LABEL_X,MAIN_LABEL_HEIGHT*i +80);
        info_element->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
        info_element->setParent(this->parent);
        info_container_list.push_back(info_element);
    }/*for*/
}

void server::show_info_list(QWidget *parent) /*******************************************************************this kind of named functions are all used to display the data list for the first time and last looked***********************************************/
{
    this->ui_page = 13;
    if(check_info_open()) return;
    get_info_list();
    for(int i = 0; i < labels_select_control;i++) info_control_list[i]->hide();
    for(int i = 0; i < info_container_list.size();i++)
        info_container_list[i]->show();
    if(this->isteacher) this->info_title->setText("教师信息页面");
    else this->info_title->setText("学生信息页面");
    this->info_search->setText("input some data to search...");
    this->info_specify_search->setText("specify search...");
    this->info_title->show();
    this->info_back->show();
    this->info_search->show();
    this->info_specify_search->show();
    this->info_next->show();
    this->info_last->show();
    this->parent->update();
    this->is_info_page = 1;
}

void server::switch_info_list(QWidget *parent)
{
    this->hide_all(parent,13);
    get_info_list();
    for(int i = 0; i < info_container_list.size();i++)
        info_container_list[i]->show();
    this->info_title->show();
    this->info_back->show();
    this->info_search->show();
    this->info_specify_search->show();
    this->info_next->show();
    this->info_last->show();
    this->parent->update();
    if(is_info_page_open)
        show_info_page(parent);
}

void server::show_search()
{

}

void server::show_specify_search()
{

}



void server::back_info_main()
{
    this->is_info_page = 0;
    this->is_info_page_open = 0;
    this->is_info_page_content_open = 0;
    hide_all(parent,13);
    show_info_main(this->parent);
}

void server::show_add_main(QWidget* parent)
{
    init_add();
    for(int i = 0; i < labels_add_control;i++)
    {
        add_control_list[i]->show();
    }
}


void server::get_add_list()
{
    QString sql;
    if(is_add_class)
    {
        sql = "SELECT class_id,stu_num,head_teacher_id,head_stu_id from classes;";

    }else {
        sql = "SELECT course_id, course_name from courses;";

    }

    int x, y;
    release_ui_vector2(this->add_container_list);/***********************************release the memory of add_container_list object*************************************/
    QVector<QVector<QString>> result = message_serialization::analysis_content_2(this->dbo->select(sql));
    for(int i = 0; i < result.size();i++)
    {
        QVector<label_editable*> temp;
        y =i * (MAIN_LABEL_HEIGHT + ADD_LIST_INTERVAL_X) + ADD_LIST_INTERVAL_X;
        for(int j = 0; j < result[i].size();j++)
        {
            label_editable* label = new label_editable();
            x = j * (MAIN_LABEL_WIDTH + ADD_LIST_INTERVAL_X) + ADD_LIST_INTERVAL_X;
            if(result[i][j] == "0" || result[i][j] == "") label->setText("双击修改信息");
            else label->setText(result[i][j]);
            label->text_origin = label->text();
            label->move(x,y);
            label->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
            temp.push_back(label);
        }
        add_container_list.push_back(temp);
    }
    if(scroll_add_list) {delete scroll_add_list; scroll_add_list = nullptr;}
    scroll_add_list = new scroll_label(this->parent,&add_container_list);
    scroll_add_list->setFixedSize(500,540);
    scroll_add_list->move(300,30);
}

void server::show_add_opt_list(QWidget* parent)
{

    if(check_add_open()) return;
    get_add_list();
    show_add_main(parent);
    show_add_list(parent);
}

void server::show_add_list(QWidget *parent)
{
    int i; int j;
    for(i = 0; i < add_container_list.size();i++)
   {
        for(j = 0; j < add_container_list[i].size();j++)
         {
             add_container_list[i][j]->show();
        }
    }
    scroll_add_list->show();
    this->add_ok->show();
    this->add_no->show();
    this->add_add->show();
    if(this->is_add_class)for(i = 0; i < classes.size(); i++) classes[i]->show();
    else for(i = 0; i < courses.size();i++) {courses[i]->show();}
     is_add_page_open = 1;
     this->parent->update();
}

void server::switch_add_list(QWidget* parent)/**************************prepared for scroll line****************************************/
{

    get_add_list();
    show_add_main(parent);
    show_add_list(parent);
}

void server::add_new_list()
{


    QVector<label_editable*> labels;
    int rows = add_container_list.size();
    for(int i = 0; i < add_container_list[0].size();i++)
    {
        label_editable* label = new label_editable();
        label->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
        label->move(i * (MAIN_LABEL_WIDTH+ADD_LIST_INTERVAL_X) + ADD_LIST_INTERVAL_X,add_container_list[rows-1][0]->y() + MAIN_LABEL_HEIGHT + ADD_LIST_INTERVAL_X + ADD_LIST_INTERVAL_X);
        label->added = true;
        label->setText("双击修改信息");
        label->setParent(this->scroll_add_list);
        labels.push_back(label);
    }
    add_container_list.push_back(labels);
    for(int i = 0; i < labels.size();i++)
    {
        labels[i]->show();
    }
    this->parent->update();
    if(labels[0]->y() >= 570) scroll_to_bottom(add_container_list);
}

void server::agree_add()
{

    QVector<QString> update_add_information;
    QString sql;
    bool availiable = false;
    if(this->is_add_class)
    {
        QString attributes_class[4] = {"class_id","stu_num","head_teacher_id","head_stu_id"};
        for(int i = 0; i < add_container_list.size(); i++)
        {
           if(add_container_list[i][0]->added)
           {

                sql = "INSERT INTO classes values(";
                for(int j = 0; j < add_container_list[i].size();j++)
                {
                    sql += add_container_list[i][j]->text();
                    sql += ",";
                }
                sql[sql.length() - 1] = ' ';
                sql += ");";
                update_add_information.push_back(sql);
               continue;
           }
            sql = "UPDATE classes SET ";
           for(int j = 0; j < add_container_list[i].size();j++)
            {
               if(add_container_list[i][j]->edited)
                   {        availiable = true;
                             sql  += attributes_class[j]; sql += "=";
                             sql += add_container_list[i][j]->text();
                             sql += ",";
               }
           }
           sql[sql.length() -1] = ' ';
           sql += "WHERE class_id = ";
           sql += add_container_list[i][0]->text_origin;
           sql += ";";
           if(availiable){update_add_information.push_back(sql); availiable = false;}


        }/*for*/
    }
    else {
         QString attributes_course[2] = {"course_id","course_name"};
         for(int i = 0; i < add_container_list.size(); i++)
         {
            if(add_container_list[i][0]->added)
            {
                 sql = "INSERT INTO courses values(";
                 sql += add_container_list[i][0]->text();
                 sql += ", '";
                 sql += add_container_list[i][1]->text();
                 sql += "');";
                 update_add_information.push_back(sql);
                continue;
            }

            sql = "UPDATE courses SET ";
           for(int j = 0; j < add_container_list[i].size();j++)
            {
               if(add_container_list[i][j]->edited)
                   {        availiable = true;
                     if(j == 0)
                             {sql  += attributes_course[j]; sql += "=";
                             sql += add_container_list[i][j]->text();
                             sql += ",";}
                       else {sql += attributes_course[j]; sql += "='";
                         sql += add_container_list[i][j]->text();
                         sql += "',";}

                     }
               }
           sql[sql.length() -1] = ' ';
           sql += "WHERE course_id = ";
           sql += add_container_list[i][0]->text_origin;
           sql += ";";
           if(availiable){update_add_information.push_back(sql); availiable = false;}
         }
    }
    qDebug() << update_add_information.size();
    for(int i = 0; i < update_add_information.size();i++)
    {
        this->dbo->update(update_add_information[i]);
        qDebug() << update_add_information[i];
    }

    for(int i = 0; i < add_container_list.size();i++)
    {
        for(int j = 0; j < add_container_list[i].size();j++)
        {
            add_container_list[i][j]->edited = false;
            add_container_list[i][j]->added = false;
        }
    }

}

void server::disagree_add()
{
    for(int i = 0; i < add_container_list.size();i++)
    {
        for(int j = 0; j < add_container_list[i].size();j++)
        {
            add_container_list[i][j]->setText(add_container_list[i][j]->text_origin);
            add_container_list[i][j]->store= true;
            add_container_list[i][j]->show();
        }
    }
}
void server::update_online_num() /*show the numer of online people*/
{
    show_online_num(parent);
}

void server::init_main()
{
    this->labels[0]->move(OPT_LABEL_X,OPT_LABEL_Y);
    this->labels[1]->move(MSG_LABEL_X,MSG_LABEL_Y);
    this->labels[2]->move(SHUTDOWN_LABEL_X,SHUTDOWN_LABEL_Y);
    this->labels[3]->move(MSG_LABEL_X,MAIN_INFO_LABEL_Y);
    this->labels[4]->move(ADD_LABEL_X,ADD_LABEL_Y);
    for(int i = 0; i < labels_main_list;i++)
    {
         this->labels[i]->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
         QObject::connect(this->labels[i],SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
         this->labels[i]->setParent(parent);
    }
}

/*associated with one existing msg element of msg container list always*/
void server::print_msg_page(int index)
{
    message* msg = this->socket_server->unproced_messages.at(index); /*the smallest index of msg list label*/
    print_msg_page_update(msg);
}

void server::print_msg_page_update(message* msg) /*show the content of update type message*/
{
    this->unproced_msg_content_top->setText(message_serialization::int2str(msg->sender));
    this->unproced_msg_content_main->setText(msg->content);
    this->cur_msg = msg;
    this->unproced_msg_content_top->show();
    this->unproced_msg_content_main->show();
    this->msg_ok->show();
    this->msg_no->show();
    this->is_msg_page_open = 1;
}

void server::print_server_info_log(int id) /*print the new connection info and do other things*/
{
    QString str = "id: ";
    str += msg_serial.int2str(id);
    QTcpSocket* socket = this->socket_server->get_client(id);
    str += "connected in port: ";
    str += msg_serial.int2str(socket->peerPort());
    message* msg = new message(0,id,0,0,0,str);
    this->log_message->push_back(msg);
    qDebug() << msg->content;//todo
    this->parent->update();
}

void server::get_info_page(int id)
{
    QString sql = "";
    QString t_id = info_container_list.at(id)->id;
    release_ui_editable(info_page_content_profile);
    release_ui_vector2(info_page_content_exam);
    if(isteacher)
    {
        sql = "SELECT t.teacher_id,t.name,t.gender,t.age,t.tel,t.duties,courses.`course_name`,teacher_class.`class_id` FROM (SELECT * FROM teachers WHERE teacher_id = ";
        sql += t_id;
        sql += ") AS t LEFT JOIN courses ON courses.course_id = t.course_id \
            LEFT JOIN teacher_class ON t.teacher_id = teacher_class.`teacher_id`;";
    }
    else{
        sql = "SELECT a.stu_id,a.stu_name,a.gender,a.age,a.tel,a.class_id,examination.exam_name,courses.course_name,study_record.grade,teachers.name,examination.exam_id,courses.course_id,teachers.teacher_id FROM (SELECT stu_id,stu_name,gender,age,tel,class_id FROM students WHERE stu_id = ";
        sql += t_id;
        sql += ") AS a LEFT JOIN study_record ON a.stu_id = study_record.stu_id \
LEFT JOIN examination ON study_record.exam_id = examination.exam_id \
LEFT JOIN courses ON study_record.course_id = courses.course_id \
LEFT JOIN teachers ON teachers.teacher_id = study_record.teacher_id;";
    }
    QVector<QVector<QString>> result = message_serialization::analysis_content_2(this->dbo->select(sql));
    if(isteacher) set_info_page(result,7);
    else set_info_page(result,6);
}



/*************************************************************return to this function after executing set_info_page*************************************************************************/














void server::set_info_page(QVector<QVector<QString>> &result,int profile)
{
    if(scroll_info_exam) {delete scroll_info_exam; scroll_info_exam = nullptr;}
    /*generate the information of the student*/
    int line_1_x = 310; int w = 30; int line_1_y = 70; int line_2_y = 110; int line_3_y = 150; int line_4_y = 190;
    int stx = line_1_x + w +10 + 10;
    for(int i = 0; i < profile;i++)
    {
        label_editable* label = new label_editable();
        label->setParent(this->parent);
        label->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
        if(result[0][i] == "" || result[0][i] == "0") label->setText("双击修改信息");
        else label->setText(result[0][i]);
        label->text_origin = label->text();
        info_page_content_profile.push_back(label);
    }
    if(isteacher)
    {
        info_page_content_profile[0]->move(stx,line_1_y); info_page_content_profile[1]->move(stx + MAIN_LABEL_WIDTH+ADD_LIST_INTERVAL_X* 2+ w,line_1_y);
        info_page_content_profile[2]->move(stx + MAIN_LABEL_WIDTH+ADD_LIST_INTERVAL_X*2 + w,line_2_y); info_page_content_profile[3]->move(stx,line_2_y);
        info_page_content_profile[4]->move(stx,line_3_y);
        info_page_content_profile[5]->move(stx,line_4_y); info_page_content_profile[6]->move(stx + MAIN_LABEL_WIDTH+ADD_LIST_INTERVAL_X*2 + w,line_3_y +40);
        id->setText("工号");
    }else {
        id->setText("学号");
        info_page_content_profile[0]->move(stx,line_1_y); info_page_content_profile[1]->move(stx + MAIN_LABEL_WIDTH+ADD_LIST_INTERVAL_X* 2+ w,line_1_y);
        info_page_content_profile[2]->move(stx + MAIN_LABEL_WIDTH+ADD_LIST_INTERVAL_X*2 + w,line_2_y); info_page_content_profile[3]->move(stx,line_2_y);
        info_page_content_profile[4]->move(stx,line_3_y);
        info_page_content_profile[5]->move(stx,line_4_y);
    }
    /*generate the course grade of the student selected*/

    for(int i = 0; i < result.size();i++)
    {

        QVector<label_editable*> labels;
        for(int j = profile; j < result[i].size();j++)
        {
            label_editable* label = new label_editable();
            label->move((j - profile) * (60+ADD_LIST_INTERVAL_X + 20),i*(MAIN_LABEL_HEIGHT+ADD_LIST_INTERVAL_X));
            label->setFixedSize(80,MAIN_LABEL_HEIGHT);

            if(result[i][j] == "" || result[i][j] == "0")
                    label->setText("无");
                else label->setText(result[i][j]);
            label->text_origin = label->text();
            labels.push_back(label);
        }
         info_page_content_exam.push_back(labels);
    }

    scroll_info_exam = new scroll_label( this->parent,&info_page_content_exam);
    scroll_info_exam->move(10 + SEC_LEVEL_PAGE_X,300);
    scroll_info_exam->setFixedSize(500,270);
}


void server::show_info_page(QWidget* parent)
{
scroll_info_exam->show();
  for(int i = 0;  i < info_page_content_profile.size();i++) info_page_content_profile[i]->show();



    if(isteacher)
    {
        for(int i = 0; i < teacher_info_attri.size();i++) teacher_info_attri[i]->show();
    }else {
         for(int i = 0; i < student_info_attri.size();i++) student_info_attri[i]->show();
    }
    this->info_ok->show();this->info_no->show();info_add_new->show();
    this->parent->update();
    is_info_page_open = 1;
}

void server::print_info_page(int id)
{
    get_info_page(id);
    show_info_page(parent);
}

void server::info_generate_update()
{
    QVector<QString> update_information;
     QString sql;
     bool availiable = false;
    if(isteacher)
    {

       QString attributes[8] = {"teacher_id","name","gender","age","tel","duties","course_name","class_id"};
        /*generate the sql senctenct that update the teacher's teac class*/
       for(int i = 0; i < info_page_content_exam.size();i++)
        {
            for(int j = 0; j < info_page_content_exam[i].size();j++)
            {
                if(info_page_content_exam[i][j]->added)
                {
                     sql = "INSERT INTO teacher_class values( ";
                     sql += info_page_content_profile[0]->text();
                     sql += ",";
                     sql += info_page_content_exam[i][j]->text();
                     sql += ");";
                     update_information.push_back(sql);
                     continue;
                }
                if(info_page_content_exam[i][j]->edited)
                {
                    sql = "UPDATE teacher_class SET class_id = ";
                    sql += info_page_content_exam[i][j]->text();
                    sql += " WHERE teacher_id = ";
                    sql += info_page_content_profile[0]->text();
                    sql += " and class_id = ";
                    sql += info_page_content_exam[i][j]->text_origin;
                    sql += ";";
                    update_information.push_back(sql);
                }
            }
        }/*for*/
        /*generate the sql sentence that update the teachers' course_id*/
        if(info_page_content_profile[6]->edited)
        {
            availiable = true;
             sql = "UPDATE teachers SET course_id = (SELECT course_id FROM courses WHERE course_name = '";
             sql += info_page_content_profile[6]->text();
             sql += "';";
        }
       if(availiable){update_information.push_back(sql); availiable = false;}

         /*generate the sql sentence that update the teachers' profile*/
        sql = "UPDATE teachers SET ";
        for(int i = 0; i < 6;i++)
        {
            if(info_page_content_profile[i]->edited)
            {
                availiable =true;
                sql += attributes[i];
                if(i == 1|| i == 2 || i == 4 || i == 5)
                {
                    sql += "='";
                    sql += info_page_content_profile[i]->text();
                    sql += "',";
                }
                else {
                    sql += "=";
                    sql += info_page_content_profile[i]->text();
                    sql += ",";
                }
            }
        }/*for used to collect profile information update*/

        sql[sql.length() - 1] = ' ';
        sql += "WHERE teacher_id = ";
        sql += info_page_content_profile[0]->text(); sql += ";";
        if(availiable){update_information.push_back(sql); availiable = false;}
    }
    else
    {
        QString attributes[18] = {"stu_id","stu_name","gender","age","tel","class_id","exam_id","course_id","grade","teacher_id","exam_name","course_name","grade", "name","examination","courses","study_record","teachers"};
        QString assist_attributes[3] = {"exam_id","course_id","teacher_id" };
        for(int i = 0; i < info_page_content_exam.size();i++)
        {
            if(info_page_content_exam[i][0]->added)
            {
                sql = "INSERT INTO study_record(exam_id, course_id, grade, teacher_id) values(";
               sql += "(SELECT exam_id FROM examination WHERE exam_name = '"; sql += info_page_content_exam[i][0]->text(); sql += "'),";
               sql += "(SELECT course_id FROM courses WHERE course_name = '"; sql += info_page_content_exam[i][1]->text(); sql += "),";
               sql += info_page_content_exam[i][2]->text(); sql += ",";
               sql += "(SELECT teacher_id FROM teahcers WHERE teacher_name = "; sql += info_page_content_exam[i][3]->text(); sql += "));";
               update_information.push_back(sql);
               continue;
            }
            sql = "UPDATE study_record SET ";
            int x = 4;
            for(int j = 0; j < info_page_content_exam[i].size() - 3;j++)
            {
                if(info_page_content_exam[i][j]->edited)
                {
                    availiable = true;
                    sql += attributes[j + 6];
                    sql += "=";
                    if(j != 2)
                    {
                         sql += "(SELECT "; sql += attributes[j+6];
                         sql += " FROM "; sql += attributes[j+14]; sql += " WHERE "; sql += attributes[j+10]; sql +="='"; sql += info_page_content_exam[i][j]->text(); sql += "'),";
                    }
                    else {
                        sql += info_page_content_exam[i][2]->text();sql += ",";
                    }
                }
            }
            sql[sql.length() -1] = ' ';
            sql += " WHERE  ";
            for(int j = 4; j < info_page_content_exam[i].size();j++)
            {
                sql +=assist_attributes[j-4];
                sql += "=";
                sql += info_page_content_exam[i][j]->text();
                sql += " and ";
            }
            sql[sql.length() - 5] = ';';
            sql[sql.length()-4] = 0;
            if(availiable){ update_information.push_back(sql); availiable =false;}
        }
        sql = "UPDATE students SET ";
        for(int i = 0; i < 6;i++)
        {
            if(info_page_content_profile[i]->edited)
            {
                availiable =true;
                sql += attributes[i];
                if(i == 0 || i == 3 || i == 5)
                {
                     sql += " = ";
                     sql += info_page_content_profile[i]->text();
                     sql += ",";
                }else
                {
                    sql += "= '";
                    sql += info_page_content_profile[i]->text();
                    sql += "',";
                }
            }
        }/*for*/
        sql[sql.length() - 1] = ' ';
        sql += "WHERE stu_id = "; sql += info_page_content_profile[0]->text(); sql += ";";
        if(availiable) {update_information.push_back(sql); availiable = false;}

    }

    for(int i = 0; i < update_information.size(); i++)
    {
        qDebug() << "teacher, student update sql sentences: " << update_information[i];
        if( this->dbo->update(update_information[i]) >= 0 )
        {
            for(int i = 0; i < info_page_content_exam.size();i++)
            {
                for(int j = 0; j < info_page_content_exam[i].size();j++)
                {
                    this->info_page_content_exam[i][j]->text_origin = info_page_content_exam[i][j]->text();
                    this->info_page_content_exam[i][j]->edited = false;
                    this->info_page_content_exam[i][j]->added = false;
                }
            }
        }
    }

}

void server::scroll_to_bottom(QVector<QVector<label_editable*>>& container)
{
    int rows = container.size();
    int columns = container[0].size();
    while(container[rows-1][columns-1]->y() >= 510)
    {
        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < columns;j++)
            {
                container[i][j]->move(container[i][j]->x(), container[i][j]->y() -24);
                container[i][j]->show();
            }
        }
    }
    this->parent->update();
}

void server::info_new_list()
{
    qDebug() << "info_new list ";
    QVector<label_editable*> labels;

    int rows = info_page_content_exam.size(); int columns = this->isteacher ? 1:4;
    qDebug() << "row: " << rows << " columns " << columns;
    int w = 80;
    label_editable* label;
    int y = rows == 0 ? 0 : info_page_content_exam[rows-1][0]->y() + (MAIN_LABEL_HEIGHT +ADD_LIST_INTERVAL_X);
    for(int i = 0; i < columns; i++)
    {
        label = new label_editable();
        label->setText("双击修改信息");
        label->text_origin= "双击修改信息";
        label->added=true;
        label->setParent(this->scroll_info_exam);
        label->setFixedSize(w,MAIN_LABEL_HEIGHT);
        label->move( i * (w + ADD_LIST_INTERVAL_X + 10) ,y ); // dynamic add the position to the new list
        labels.push_back(label);
    }
    info_page_content_exam.push_back(labels);
    for(int i = 0; i <info_page_content_exam[rows].size();i++)
    {
        info_page_content_exam[rows][i]->show();
    }
    if(label->y() >= 510) scroll_to_bottom(info_page_content_exam);
}
void server::agree_info()
{
    info_generate_update();
}
void server::disagree_info()
{
    for(int i = 0; i < info_page_content_profile.size();i++)
    {
        info_page_content_profile[i]->setText(info_page_content_profile[i]->text_origin);
        info_page_content_profile[i]->store =true;
        info_page_content_profile[i]->show();
    }

    for(int i = 0; i < info_page_content_exam.size();i++)
    {
        for(int j = 0; j < info_page_content_exam[i].size();j++)
        {
            info_page_content_exam[i][j]->setText(info_page_content_exam[i][j]->text_origin);
            info_page_content_exam[i][j]->store =true;
            info_page_content_exam[i][j]->show();
        }
    }
}
void server::hide_all(QWidget *parent,int instruct)
{
    int i;
    i = instruct == -1 ? this->last_page : instruct;
    switch(i)
    {
    case 0:
        for(i = 0; i < all_server_controls.size();i++) this->all_server_controls[i]->hide();
        break;
    case 1:
        for(i = 0; i < msg_container_list.size();i++)this->msg_container_list[i]->hide();
         for(i = 0; i < all_msg_controls.size();i++) this->all_msg_controls[i]->hide();
        break;
    case 2:
        break;
    case 13:
        for(i = 0; i < all_info_controls.size();i++) this->all_info_controls[i]->hide();
        for(i = 0; i < info_container_list.size();i++)this->info_container_list[i]->hide();
        for(i = 0; i <  info_page_content_profile.size();i++)  info_page_content_profile[i]->hide();
        if(scroll_info_exam) scroll_info_exam->hide();
        for( i = 0; i < teacher_info_attri.size(); i++) teacher_info_attri[i]->hide();
        for( i = 0; i < student_info_attri.size();i++) student_info_attri[i]->hide();
        info_search->hide();info_specify_search->hide();
        break;
    case 25:
         for(i = 0; i < all_add_controls.size();i++)this->all_add_controls[i]->hide();
         if(scroll_add_list) scroll_add_list->hide();
         for(i = 0; i < classes.size(); i++) classes[i]->hide();
         for(i = 0; i < courses.size(); i++) courses[i]->hide();
        break;
    }
}

void server::clear_container_list(QVector<MLabel*>& container)
{
    while(container.size() > 0)
    {
        container.first()->close();
        delete container.first();
        container.removeFirst();
    }
}

bool server::check_server_open()
{
    if(!is_server_open)
        return false;
    for(int i =0 ; i < labels_server_control;i++) server_control[i]->show();
    QString str = "";
    for(int i = 0; i < log_message->size();i++)
        str += log_message->at(i)->content;
    server_info_log->setText(str);
    server_info_log->show();
    return true;
}

bool server::check_info_open()
{
    if(!is_info_page) return false;
    info_title->show();
    info_back->show();info_search->show();info_specify_search->show();
    for(int i = 0; i < info_container_list.size();i++)
        info_container_list[i]->show();
    info_next->show();info_last->show();
    if(is_info_page_open)
    {
        show_info_page(parent);
    }
    return true;
}

bool server::check_msg_open()
{
    if(!is_msg_open) return false;
    for(int i = 0; i < msg_container_list.size();i++) msg_container_list[i]->show();
    this->last->show();this->next->show();
    if(this->is_msg_page_open){
        this->unproced_msg_content_main->show();
        this->unproced_msg_content_top->show();
        this->msg_ok->show();
        this->msg_no->show();
    }
    return true;
}

bool server::check_add_open()
{
    if(!is_add_page_open) return false;
    int i;
    for(i = 0; i < all_add_controls.size();i++)
        all_add_controls[i]->show();
    show_add_list(parent);
    return true;
}

QVector<QString> server::analysis_message(message msg)
{
    QVector<QString> vector;
    QString str = "";
    if(msg.type == 0 || msg.type == 3)
    {
        for(int i = 0; i < msg.content.length();i++)
        {
            QCharRef c = msg.content[i];
            if(c != SEG_ONE_LINE)
                str+=msg.content[i];
            else
            {
                vector.push_back(str);
                str = "";
                if(msg.content[i+1] == SEG_SWT_LINE) i++;
            }
        }
    }
    return vector;
}

int i = 0;

void server::test_db()
{



}

void server::main_process(int index)
{
    //test_db();
    this->last_page = this->ui_page;
    if(index == this->ui_page) return;
    switch(index)
    {
    case 0:/*show server page*/
        this->ui_page = 0;
        hide_all(this->parent);
        this->parent->update();
        show_ser_list(this->parent);

        break;
    case 1:/*show msg page*/
        this->ui_page = 1;
        hide_all(this->parent);
        this->parent->update();
        show_msg_list(this->parent);
        break;
    case 2:/*exit*/
        this->server_end();
        QApplication::exit();
        break;
    case 3:
        qDebug() << "starting the server... "; //todo
        this->server_start();
        break;
    case 4:
        qDebug() << "ending the server...";
        this->server_end();
        break;
    case 9:/*last page*/
        this->ui_page = 1;
        hide_all(this->parent);
        if(this->curend_msg < MSG_LIST_NUM) return;
            this->curend_msg -=MSG_LIST_NUM;
        qDebug() << this->curend_msg;
        this->switch_msg_list(this->parent);
        break;
    case 10:/*next page control*/
        this->ui_page =1;
        hide_all(parent);
        this->curend_msg += MSG_LIST_NUM;this->switch_msg_list(this->parent); qDebug() << this->curend_msg;
        break;
    case 11:/*agree control*/ /*via 11, 12 label to manage the message send from clients*/
        this->ui_page = 1;
        agree_msg();
        break;
    case 12:/*disagree control*/
        this->ui_page = 1;
        disagree_msg();
        break;
    case 13:/*info select option*/
        this->ui_page = 13;
        hide_all(this->parent);
         if(this->is_info_page)
         {   show_info_list(this->parent); /*represent there is the opend page in memory*/
            return;}
        this->parent->update();
        show_info_main(this->parent);
        break;
    case 14:
        this->ui_page = 13;
        this->isteacher = 1;
        show_info_list(this->parent);
        break;
    case 15:
        this->ui_page = 13;
        this->isteacher = 0;
        show_info_list(this->parent);
        break;
    case 16:
        this->ui_page = 13;
        if(this->curend_info < INFO_LIST_NUM) return;
            this->curend_info -=INFO_LIST_NUM;
        this->switch_info_list(this->parent);
        break;
    case 17:
        this->ui_page = 13;
        this->curend_info += INFO_LIST_NUM;
        this->switch_info_list(this->parent);
        break;
    case 19:
        this->ui_page = 13;
        this->show_search();
        break;
    case 20:
        this->ui_page = 13;
        this->show_specify_search();
        break;
    case 22:
        this->ui_page = 13;
        agree_info();
        break;
    case 23:
        this->ui_page = 13;
        disagree_info();
        break;
    case 24:
        this->ui_page = 13;
        this->hide_all(this->parent);
        back_info_main();
        break;
    case 25:/*add page*/
        this->ui_page =25;
        this->hide_all(this->parent);
        this->parent->update();
        if(is_add_page_open)
        {
            show_add_opt_list(parent);
            return;
        }
        this->show_add_main(parent);
        break;
    case 26:
        this->ui_page =25;
        this->hide_all(this->parent);
        this->parent->update();
        this->is_add_class = 1;
        switch_add_list(parent);
        break;
    case 27:
        this->ui_page = 25;
        this->hide_all(this->parent);
        this->parent->update();
        this->is_add_class=0;
        switch_add_list(parent);
        break;
    case 28:
        this->ui_page = 25;
        agree_add();
        break;
    case 29:
        this->ui_page = 25;
        disagree_add();
        break;
    case 30:
        this->ui_page = 25;
        add_new_list();
        break;
    case 31:/*add new info*/
        info_new_list();
        break;
        default:
        break;
    }
}

void server::release_ui_vector(QVector<MLabel*> &controls)
{
    while(controls.size() > 0)
    {
        delete controls.first();
        controls.removeFirst();
    }
}

void server::release_ui_editable(QVector<label_editable *> &controls)
{
    while(controls.size() > 0)
    {
        delete controls.first();
        controls.removeFirst();
    }
}

void server::release_ui_vector2(QVector<QVector<label_editable*>>&controls)
{
    while(controls.size() > 0)
    {
        while(controls[0].size() > 0)
        {
            delete controls[0].first();
            controls[0].removeFirst();
        }
        controls.removeFirst();
    }
}

void server::init_info()
{
    if(all_info_controls.size() > 0) return;
    this->info_control_list[0]->move(INFO_TEACHER_LABEL_X,INFO_TEACHER_LABEL_Y);
    this->info_control_list[1]->move(INFO_STUDENT_LABEL_X,INFO_STUDENT_LABEL_Y);
    this->info_next->move(INFO_NEXT_LABEL_X,INFO_NEXT_LABEL_Y);
    this->info_last->move(INFO_LAST_LABEL_X,INFO_NEXT_LABEL_Y);
    this->info_title->move(INFO_SELECT_LABEL_X,INFO_SELECT_LABEL_Y);
    this->info_search->move(INFO_SEARCH_LABEL_X,INFO_SEARCH_LABEL_Y);
    this->info_specify_search->move(INFO_SPECI_SELECT_X,INFO_SPECI_SELECT_Y);
    this->info_back->move(INFO_BACK_LABEL_X,INFO_SEARCH_LABEL_Y);
    this->info_ok->move(INFO_OK_LABEL_X,MSG_AGREE_LABEL_Y);
    this->info_no->move(INFO_NO_LABEL_X,MSG_AGREE_LABEL_Y);
    this->info_add_new->move(500,570);
    this->info_next->setParent(this->parent);
    this->info_last->setParent(this->parent);this->info_add_new->setParent(this->parent);
    this->info_title->setParent(this->parent);this->info_search->setParent(this->parent);this->info_specify_search->setParent(this->parent);
    this->info_back->setParent(this->parent);
    this->info_ok->setParent(this->parent);
    this->info_no->setParent(this->parent);
    this->info_next->setFixedSize(INFO_NEXT_LABEL_WIDTH,MAIN_LABEL_HEIGHT);this->info_last->setFixedSize(INFO_NEXT_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
    this->info_title->setFixedSize(INFO_SELECT_LABEL_WIDTH,INFO_SELECT_LABEL_HEIGHT);this->info_search->setFixedSize(INFO_SEARCH_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
    this->info_specify_search->setFixedSize(INFO_SPECI_SELECT_WIDTH,MAIN_LABEL_HEIGHT);this->info_specify_search->setFixedSize(INFO_SPECI_SELECT_WIDTH,MAIN_LABEL_HEIGHT);
    this->info_ok->setFixedSize(MSG_NEXT_LABEL_WIDTH,MSG_NEXT_LABEL_HEIGHT);this->info_no->setFixedSize(MSG_NEXT_LABEL_WIDTH,MSG_NEXT_LABEL_HEIGHT);
    this->info_back->setFixedSize(INFO_BACK_LABEL_WIDTH,MAIN_LABEL_HEIGHT); this->info_add_new->setFixedSize(INFO_BACK_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
    QObject::connect(this->info_next,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(this->info_last,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(this->info_title,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
QObject::connect(this->info_add_new,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(this->info_ok,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(this->info_no,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(this->info_back,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    for(int i = 0; i < labels_select_control;i++)
    {
        this->info_control_list[i]->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
        this->info_control_list[i]->setParent(this->parent);
        QObject::connect(info_control_list[i],SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
        all_info_controls.push_back(info_control_list[i]);
    }

    all_info_controls.push_back(info_next);
    all_info_controls.push_back(info_last);
    all_info_controls.push_back(info_title);
    all_info_controls.push_back(info_back);
    all_info_controls.push_back(info_ok);
    all_info_controls.push_back(info_no);
all_info_controls.push_back(info_add_new);
    id->move(310,70); name->move(470,70);age->move(310,110);gender->move(470,110);
    tel->move(310,150);duties->move(310,190); teacher_course->move(470,190);
    student_class->move(310,190);
    teacher_class->move(350,270);
    exam_name->move(310,270); student_course->move(400,270); grade->move(490,270);teacher->move(570,270);
    teacher_info_attri.push_back(id); teacher_info_attri.push_back(name); teacher_info_attri.push_back(age); teacher_info_attri.push_back(gender); teacher_info_attri.push_back(tel); teacher_info_attri.push_back(teacher_class);
    teacher_info_attri.push_back(duties); teacher_info_attri.push_back(teacher_course);
    student_info_attri.push_back(id);student_info_attri.push_back(name); student_info_attri.push_back(age); student_info_attri.push_back(gender); student_info_attri.push_back(tel);
    student_info_attri.push_back(student_class); student_info_attri.push_back(exam_name); student_info_attri.push_back(student_course);student_info_attri.push_back(grade);student_info_attri.push_back(teacher);
    for(int i = 0; i < teacher_info_attri.size();i++)
    {
        QLabel* label = teacher_info_attri[i];
        label->setParent(this->parent);
        if(label->text() == "执教课程" || label->text() == "执教班级")label->setFixedSize(50,30);
          else  label->setFixedSize(30,30);
    }
    for(int i = 0; i < student_info_attri.size();i++)
    {
        QLabel* label = student_info_attri[i];
        label->setParent(this->parent);
        if(label->text() == "课程名称" || label->text() == "考试名称" || label->text() == "授课教师")label->setFixedSize(50,30);
          else  label->setFixedSize(30,30);
    }
}

void server::init_add()
{
    if(all_add_controls.size() > 0) return;
    add_control_list[0]->move(ADD_CLASS_CONTROL_X,ADD_CLASS_CONTROL_Y);
    add_control_list[1]->move(ADD_COURSE_CONTROL_X,ADD_COURSE_CONTROL_Y);
    add_ok->move(INFO_OK_LABEL_X,ED_LABEL_Y);
    add_no->move(INFO_NO_LABEL_X,ED_LABEL_Y);
    add_add->move(ADD_ADD_LABEL_X,ADD_ADD_LABEL_Y);
    add_no->setParent(this->parent);add_ok->setParent(this->parent); add_add->setParent(this->parent);
    add_ok->setFixedSize(ADD_LIST_WIDTH,MAIN_LABEL_HEIGHT);
    add_no->setFixedSize(ADD_LIST_WIDTH,MAIN_LABEL_HEIGHT);
    add_add->setFixedSize(ADD_LIST_WIDTH,MAIN_LABEL_HEIGHT);
    QObject::connect(add_ok,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(add_no,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(add_add,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    all_add_controls.push_back(add_ok);
    all_add_controls.push_back(add_no);
    all_add_controls.push_back(add_add);
    for(int i = 0; i < labels_add_control;i++)
    {
            add_control_list[i]->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
            add_control_list[i]->setParent(parent);
            QObject::connect(this->add_control_list[i],SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
            all_add_controls.push_back(add_control_list[i]);
    }
    courses.push_back(course_id); courses.push_back(course_name);
    classes.push_back(class_id); classes.push_back(stu_num); classes.push_back(head_teacher_id); classes.push_back(head_student_id);
    for(int i = 0; i < courses.size();i++)
    {
        courses[i]->setParent(this->parent);
        courses[i]->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
        courses[i]->move(310 + i* 120,0);
    }
    for(int i = 0; i < classes.size();i++)
    {
        classes[i]->setParent(this->parent);
        classes[i]->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
        classes[i]->move(310 + i* 120,0);
    }

}

void server::init_server()
{
    if(all_server_controls.size() > 0) return;
    this->server_control[0]->move(ST_LABEL_X,ST_LABEL_Y);
    this->server_control[1]->move(ED_LABEL_X,ED_LABEL_Y);
    this->server_info_log->move(INFO_LABEL_X,INFO_LABEL_Y);
    for(int i = 0; i < labels_server_control;i++)
    {
        this->server_control[i]->setFixedSize(MAIN_LABEL_WIDTH,MAIN_LABEL_HEIGHT);
        this->server_control[i]->setParent(this->parent);
        QObject::connect(this->server_control[i],SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
        all_server_controls.push_back(this->server_control[i]);
    }
    this->server_info_log->setParent(parent);
    this->server_info_log->setFixedSize(SERVER_INFO_LABEL_WIDTH,SERVER_INFO_LABEL_HEIGHT);
    all_server_controls.push_back(server_info_log);
}

void server::init_msg()
{
    if(all_msg_controls.size() > 0) return;

    this->last->move(MSG_LAST_LAEBL_X,MSG_NEXT_LABEL_Y); this->next->move(MSG_NEXT_LABEL_X,MSG_NEXT_LABEL_Y);
    this->msg_ok->move(MSG_AGREE_LABEL_X,MSG_AGREE_LABEL_Y);this->msg_no->move(MSG_DISAGR_LABEL_X,MSG_AGREE_LABEL_Y);
    this->unproced_msg_content_top->move(MSG_CONTENT_TOP_X,MSG_CONTENT_TOP_Y);this->unproced_msg_content_main->move(MSG_CONTENT_MAIN_X,MSG_CONTENT_MAIN_Y);

    this->last->setParent(this->parent);this->next->setParent(this->parent);
    this->msg_ok->setParent(this->parent);this->msg_no->setParent(this->parent);
    this->unproced_msg_content_top->setParent(this->parent);this->unproced_msg_content_main->setParent(this->parent);

    this->last->setFixedSize(MSG_NEXT_LABEL_WIDTH,MSG_NEXT_LABEL_HEIGHT); this->next->setFixedSize(MSG_NEXT_LABEL_WIDTH,MSG_NEXT_LABEL_HEIGHT);
    this->msg_ok->setFixedSize(MSG_NEXT_LABEL_WIDTH,MSG_NEXT_LABEL_HEIGHT); this->msg_no->setFixedSize(MSG_NEXT_LABEL_WIDTH,MSG_NEXT_LABEL_HEIGHT);
    this->unproced_msg_content_main->setFixedSize(MSG_CONTENT_MAIN_WIDTH,MSG_CONTENT_MAIN_HEIGHT);this->unproced_msg_content_top->setFixedSize(MSG_CONTENT_MAIN_WIDTH,MAIN_LABEL_HEIGHT);

    QObject::connect(this->next,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));QObject::connect(this->last,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(this->msg_ok,SIGNAL(label_clicked(int)),this,SLOT(main_process(int))); QObject::connect(this->msg_no,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));
    QObject::connect(this->unproced_msg_content_top,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));QObject::connect(this->unproced_msg_content_main,SIGNAL(label_clicked(int)),this,SLOT(main_process(int)));

    all_msg_controls.push_back(last);all_msg_controls.push_back(next); all_msg_controls.push_back(msg_ok);all_msg_controls.push_back(msg_no);
    all_msg_controls.push_back(unproced_msg_content_top);all_msg_controls.push_back(unproced_msg_content_main);
}

void server::relase_ui_main()
{
    for(int i = 0; i < labels_main_list;i++)
    {
         this->labels[i]->close();
         delete this->labels[i];
    }
}

