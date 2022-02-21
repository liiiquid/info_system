#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QApplication>
#include <QToolBox>
#include "user.h"
#include "ui_class/mbutton.h"
#include "ui_class/mlabel.h"
#include "ui_class/mlineedit.h"
#include "ui_class/ui_config.h"
#include "ui_class/scroll_label.h"
#include "ui_class/label_editable.h"
#include "thread_about/mthread.h"

class MainWindow;
class client : public QObject
{
    Q_OBJECT
public:
    client();
    ~client();
    int isconnected = 0;
    int islogin = 0;
    int init_user = 0;
    user* cur_user =nullptr;
    QString log_str = "";

     int cur_page = -1;
     int last_page = -1;

    MainWindow* parent = nullptr;
    QString titles[4] = {"消息","联系人","成绩","我"};
    MLabel* communication = new MLabel(IDENTIFIER_MAIN,1,"消息");
    MLabel* contacts = new MLabel(IDENTIFIER_MAIN,2,"联系人");
    MLabel* grade = new MLabel(IDENTIFIER_MAIN,3,"成绩");
    MLabel* me = new MLabel(IDENTIFIER_MAIN,4,"我");
    MLabel* exit = new MLabel(IDENTIFIER_MAIN,0,"退出");
    QVector<MLabel*> container_client_main_list;

    /*communication page*/
    QTextEdit* text_input = nullptr;
    scroll_label* info_scroll_page = nullptr;
    scroll_label* info_scroll_list = nullptr;
    MLabel* info_send_btn = new MLabel(IDENTIFIER_MAIN,6,"发送");
   QHash<int,QVector<QLabel*>*> user_communication_log; //log the communication information between the users
    QVector<scroll_label*> all_info_scroll;
    QLabel* name_title = new QLabel();
    MLabel* back = new MLabel(IDENTIFIER_MAIN,7,"返回");

    /*contacts page*/

    QVector<user*> users;
    scroll_label* user_scroll_list = nullptr;
        /*user profile..*/
    QVector<QLabel*> contact_user_profile;
    QLabel* head_picture = new QLabel();
    QLabel* name = new QLabel("姓名");QLabel* id = new QLabel();QLabel* gender = new QLabel("性别"); QLabel* age = new QLabel("年龄"); QLabel* tel = new QLabel("电话"); QLabel* _class = new QLabel("班级");
    MLabel* communication_btn = new MLabel(IDENTIFIER_MAIN,5,"发消息");

    /*global*/
    QLabel* title = new QLabel();
    label_editable* search_label = new label_editable();



    /*profile page*/
    MLabel* update_profile = new MLabel(IDENTIFIER_MAIN,8,"确认修改(双击修改信息)"); MLabel* profile_btn = new MLabel(IDENTIFIER_MAIN,9,"个人信息修改");
    label_editable* picture =  new label_editable();
    label_editable* profile_name = new label_editable(); label_editable* profile_id = new label_editable(); label_editable* profile_gender = new label_editable();
    label_editable* profile_age = new label_editable(); label_editable* profile_tel = new label_editable(); label_editable* profile_class = new label_editable();
    QVector<label_editable*> all_profile_controls;
    /*ui area*/
     void show_client(MainWindow* parent); /*display the main page of this software*/
     void show_main_list(MainWindow* parent);
     void show_info_list(MainWindow* parent);
    void show_contact_list(MainWindow* parent);
    void show_profile_list(MainWindow* parent);
     /*ui prepared such as the initilization of these controls*/
    void init_main_list(MainWindow* parent);
    void init_contact_list(MainWindow* parent);
    void init_info_list(MainWindow* parent);
    void init_profile_list(MainWindow* parent);

    /*clear functions*/
    void clear_users_label(QVector<head_label*>& label);
    void clear_users(QVector<user*>* users);
    void hide_all(MainWindow* parent,int last_page);
/*logical process are --------------------------------------------------------------------------------------------------------------------------------------------------*/
     QTcpSocket* socket = nullptr;
     mthread* socket_thread = nullptr;
     QMutex* mutex_read;
     QQueue<message*> read_msgs;
     QQueue<message*> send_msgs;

     void main_process(int index);//send request to the server


     /*send information*/
     void get_user_info();
    void connect_server();
    void send_info_friends();
    bool client_login(message msg);
    bool client_register(message msg);
    void send_profile_info();
    void change_profile();
    /*process information*/
    void show_data_prepare(message* mg);

    void get_profile(message* mg);
    void get_users_name(message* mg);
    void get_class_student(message* mg);
    void get_user_profile(message* mg);
    head_label*  check_send_page_open(int id);
    void open_send_page(int id);
signals:
    void write_to_server(message* m);/*via this signal to notify the socket write data to server*/
    void write_to_server_2(QVector<message*>* mgs);
    void update_user_scroll_list(head_label* ellm);
public slots:
    void show_data();
    void reconnect_server();
    void get_contact_info(head_label* ellm);
    void set_send_page(head_label* ellm);
};

#endif // CLIENT_H
