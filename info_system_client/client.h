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
    QVector<MLabel*> all_communication_list;
    QVector<MLabel*> communication_teacher_name_list;
    QVector<MLabel*> communication_student_name_list;
    scroll_label<QVector<MLabel*>>* communication_list_scroll;
    QVector<MLabel*> communication_display_page;
    QHash<int,QVector<MLabel*>> communication_content_log;

    /*contacts page*/
    QVector<MLabel*> all_contacts_elements;
    QVector<MLabel*> teachers;
    QVector<MLabel*> students;
    QHash<int,QVector<QLabel*>*> contacts_message_log;

    /*ui area*/
     void show_client(MainWindow* parent); /*display the main page of this software*/
     void show_main_list(MainWindow* parent);

     /*ui prepared such as the initilization of these controls*/
    void init_main_list(MainWindow* parent);





    void hide_all(MainWindow* parent,int last_page);
/*logical process are --------------------------------------------------------------------------------------------------------------------------------------------------*/
     QTcpSocket* socket = nullptr;
     mthread* socket_thread = nullptr;
     QMutex* mutex_read;
     QQueue<message*> read_msgs;
     QQueue<message*> send_msgs;

     void main_process(int index);//send request to the server

    void connect_server();
    bool client_login(message msg);
    bool client_register(message msg);
    void show_data_prepare(message* mg);
    void get_user_info();
signals:
    void write_to_server(message* m);/*via this signal to notify the socket write data to server*/
public slots:
    void show_data();
    void reconnect_server();
};

#endif // CLIENT_H
