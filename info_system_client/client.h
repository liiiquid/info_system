#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include <QApplication>
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

    MLabel* communication = new MLabel(IDENTIFIER_MAIN,1,"消息");
    MLabel* contacts = new MLabel(IDENTIFIER_MAIN,2,"联系人");
    MLabel* grade = new MLabel(IDENTIFIER_MAIN,3,"成绩");
    MLabel* me = new MLabel(IDENTIFIER_MAIN,4,"我");
    MLabel* exit = new MLabel(IDENTIFIER_MAIN,0,"退出");
    QVector<MLabel*> container_client_main_list;

     QLabel* page_title = new QLabel();

    /*communication page*/
    QVector<MLabel*> container_client_communication_list;
    QVector<user*> communication_list;
    scroll_label* communication_scroll_list;


    /*contacts page*/
    QVector<QLabel*> contact_user_info;
    scroll_label* user_scroll_list = nullptr;
    QVector<head_label*> users_label;
    user* teacher_opt = nullptr;
    user* student_opt = nullptr;
    user* class_opt = nullptr;
    QVector<user*> users;
    QLabel* head_picture = new QLabel();
    QLabel* name = new QLabel("姓名");QLabel* id = new QLabel();QLabel* gender = new QLabel("性别"); QLabel* age = new QLabel("年龄"); QLabel* tel = new QLabel("电话"); QLabel* _class = new QLabel("班级");
    MLabel* communication_btn = nullptr;

    QString titles[4] = {"消息","联系人","成绩","我"};
    QLabel* title = new QLabel();
    label_editable* search_label = new label_editable();
    MLabel* info_send_btn;
    QTextEdit* text_input = new QTextEdit();
    QHash<int,scroll_label*>* user_communication_log; //log the communication information between the users

    /*ui area*/
     void show_client(MainWindow* parent); /*display the main page of this software*/
     void show_main_list(MainWindow* parent);
    void show_contact_list(MainWindow* parent);
     /*ui prepared such as the initilization of these controls*/
    void init_main_list(MainWindow* parent);
    void init_contact_list(MainWindow* parent);



    /*clear functions*/
    void clear_users_label(QVector<head_label*>& label);
    void clear_users(QVector<user*>& users);
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
    bool client_login(message msg);
    bool client_register(message msg);

    /*process information*/
    void show_data_prepare(message* mg);
    void get_users_name(message* mg);
signals:
    void write_to_server(message* m);/*via this signal to notify the socket write data to server*/
    void write_to_server_2(QVector<message*>* mgs);
public slots:
    void show_data();
    void reconnect_server();
    void get_contact_info(int index);
};

#endif // CLIENT_H
