#ifndef SERVER_H
#define SERVER_H
#include <QApplication>

#include <QVector>
#include <QQueue>
#include <QThreadPool>

#include "administrator.h"
#include "message.h"
#include <QHash>
#include "dispatch/message_operation.h"
#include "utils/socket_connection.h"
#include "utils/message_serialization.h"

#include "ui_class/mlabel.h"
#include "ui_class/mlineedit.h"
#include "ui_class/ui_config.h"
#include "ui_class/label_editable.h"
#include "ui_class/mlineedit.h"
#include "ui_class/scroll_label.h"
#include "dao/db_operation.h"

#include "m_network/tserver.h"


class server : public QWidget
{
    Q_OBJECT
    void test_db();
public:
    QWidget* parent;
    server();
    ~server();
public:
    /*when there are admins try to login or register, we will get their information of account, and verify the information in database via it*/
        bool admin_login(message msg);
        bool admin_register(message msg);

    /*control the server, if the server is start, then begin to listen in a port, or stop listenning*/
        bool server_start();
        bool server_end();

    /*the ui interface of server*/
        void show_server(QWidget* parent);
        void show_main_list(QWidget* parent);
        void show_ser_list(QWidget* parent);
        void show_msg_list(QWidget* parent); int curend_msg = 0; /*represent current end of msg record*/
        void hide_all(QWidget* parent,int instruct = -1);
        void show_online_num(QWidget* parent);
        void show_info_main(QWidget* parent);
        void show_info_list(QWidget* parent);
        void show_add_main(QWidget* parent);
        void show_add_opt_list(QWidget* parent);
        void show_add_list(QWidget* parent);
        void switch_msg_list(QWidget* parent);
        void switch_info_list(QWidget* parent);
        void switch_add_list(QWidget* parent);
        void show_info_page(QWidget* parent);
        void show_search();
        void show_specify_search();
        void agree_info();
        void disagree_info();
        void agree_msg();
        void disagree_msg();
        void back_info_main();
        void agree_add();
        void disagree_add();
        void add_new_list();
public:/*ui display content control preparation*/
        void print_msg_page_update(message* msg);
        void get_info_list();
        void get_msg_list();
        void get_add_list();
        void clear_container_list(QVector<MLabel*>& container);
        bool check_server_open();
        bool check_info_open();
        bool check_msg_open();
        bool check_add_open();
        void get_info_page(int id);
        void set_info_page(QVector<QVector<QString>> &result,int profile);
        void info_generate_update();
        void info_new_list();

        void scroll_to_bottom(QVector<QVector<label_editable*>>&);
private:
        QVector<QString> analysis_message(message msg);
        message* cur_msg;
public:
    QVector<administrator*> admins;
    TServer* socket_server;
    db_operation* dbo;

/*utils*/
    message_serialization msg_serial;

/*ui object*/int ui_page = -1; int last_page = -1;
#define labels_main_list 5/* 0 - server option, 1 - message btn, 2 - close server 13 - information manage 25 - add classes and courses*/
#define labels_server_control 2
#define labels_select_control 2
#define labels_add_control 2
public:
    MLabel* labels[labels_main_list] = {new MLabel(IDENTIFIER_MAIN,0,"???????????????"),new MLabel(IDENTIFIER_MAIN,1,"??????"),new MLabel(IDENTIFIER_MAIN,2,"??????"),new MLabel(IDENTIFIER_MAIN,13,"????????????"), new MLabel(IDENTIFIER_MAIN,25,"??????")};
    /*0 - server option*/int is_server_open = 0;
    MLabel* server_control[labels_server_control] = {new MLabel(IDENTIFIER_MAIN,3,"???????????????"),new MLabel(IDENTIFIER_MAIN,4,"???????????????")};
    QTextEdit *server_info_log = new QTextEdit();
    QVector<message*>* log_message;/*print in server_info_log page*/
    /*1 - message option*/

    /*output the unproced message in msg_operation object.*/ int is_msg_open = 0;int is_msg_page_open = 0;
    MLabel* unproced_msg_list = new MLabel(IDENTIFIER_MAIN,6);
    QVector<MLabel*> msg_container_list;/*the container of msg list*/ MLabel* last = new MLabel(IDENTIFIER_MAIN,9,"?????????");MLabel* next = new MLabel(IDENTIFIER_MAIN,10,"?????????"); /*last page and next page control*/
    MLabel* unproced_msg_content_top = new MLabel(IDENTIFIER_MAIN,7);
    MLabel* unproced_msg_content_main = new MLabel(IDENTIFIER_MAIN,8);
    MLabel* msg_ok = new MLabel(IDENTIFIER_MAIN,11,"??????");
    MLabel* msg_no = new MLabel(IDENTIFIER_MAIN,12,"??????");

    /*2 - close the server*/

    /*13 - information query switch*/int curend_info = 0;/*represent current end of info container*/int isteacher = 1;/*default teacher page*/int is_info_page = 0;int is_info_page_open = 0; int is_info_page_content_open = 0;
    MLabel* info_control_list[labels_select_control] = {new MLabel(IDENTIFIER_MAIN,14,"??????????????????"),new MLabel(IDENTIFIER_MAIN,15,"??????????????????")};
    MLabel* info_last = new MLabel(IDENTIFIER_MAIN,16,"?????????");MLabel* info_next = new MLabel(IDENTIFIER_MAIN,17,"?????????");
    MLabel* info_title = new MLabel(IDENTIFIER_MAIN,18);label_editable* info_search = new label_editable();
    label_editable* info_specify_search = new label_editable();
    QVector<MLabel*> info_container_list;/*the container of info list */MLabel* info_ok = new MLabel(IDENTIFIER_MAIN,22,"??????");MLabel* info_no = new MLabel(IDENTIFIER_MAIN,23,"????????????");
    MLabel* info_back = new MLabel(IDENTIFIER_MAIN,24,"??????");
    QVector<QVector<label_editable*>> info_page_content_exam;QVector<label_editable*> info_page_content_profile;
    scroll_label* scroll_info_exam = nullptr;
    QLabel* id = new QLabel(); QLabel* name = new QLabel("??????:");
    QLabel* gender = new QLabel("??????:"); QLabel* age = new QLabel("??????:");
    QLabel* tel = new QLabel("??????:"); QLabel* duties = new QLabel("??????:");
    QLabel* teacher_course = new QLabel("??????:");QLabel* teacher_class = new QLabel("????????????");MLabel* info_add_new = new MLabel(IDENTIFIER_MAIN,31,"??????");
    QLabel* student_course = new QLabel("????????????");QLabel* exam_name = new QLabel("????????????");QLabel* student_class = new QLabel("??????:");
    QLabel* grade = new QLabel("??????");QLabel* teacher = new QLabel("????????????"); QVector<QLabel*> teacher_info_attri; QVector<QLabel*> student_info_attri;

    /*1 - set the scroll_label's size; 2 - set the location of scroll_label ; 3 - reset the location of info_page_content_exam; */
    /*25 - add class and course information before update table teachers and students*/ int is_add_class = -1; QVector<MLabel*> all_add_controls; int is_add_page_open = 0;
    MLabel* add_control_list[labels_add_control] = {new MLabel(IDENTIFIER_MAIN,26,"??????????????????"),new MLabel(IDENTIFIER_MAIN,27,"??????????????????")};
    MLabel* add_ok = new MLabel(IDENTIFIER_MAIN,28,"????????????"); MLabel* add_no = new MLabel(IDENTIFIER_MAIN,29,"??????");MLabel* add_add = new MLabel(IDENTIFIER_MAIN,30,"??????");
    QVector<QVector<QString>> add_select_info; QVector<QVector<label_editable*>> add_container_list;
    scroll_label* scroll_add_list = nullptr;
    QLabel* course_id = new QLabel("????????????"); QLabel* course_name = new QLabel("????????????");
    QLabel* class_id = new QLabel("??????"); QLabel* stu_num = new QLabel("????????????"); QLabel* head_teacher_id = new QLabel("?????????"); QLabel* head_student_id = new QLabel("??????");
    QVector<QLabel*> classes; QVector<QLabel*> courses;
public slots:
    void main_process(int);/*dispatch the control signal of main page*/
    void print_msg_page(int);/*show detailed information of the message*/
    void print_server_info_log(int id);/*print server control infomation when running*/
    void update_msg_list();/*called when unproced message is changed*/
    void update_online_num();/*called when new client connect successfully*/
    void print_info_page(int);
    void update_server_info_log(int);
public:/*init all the ui controls*/
    QVector<MLabel*> all_server_controls;
    QVector<MLabel*> all_msg_controls;
    QVector<MLabel*> all_info_controls;
    void init_main();
    void init_server();
    void init_msg();
    void init_info();
    void init_add();
    void relase_ui_main();
    void release_ui_vector(QVector<MLabel*>&);
    void release_ui_editable(QVector<label_editable*>&);
    void release_ui_vector2(QVector<QVector<label_editable*>> &controls);
};

#endif // SERVER_H
