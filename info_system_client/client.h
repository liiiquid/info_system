#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>

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
    user* cur_user = nullptr;
    QString log_str = "";
    MLabel* load_log = new MLabel('1',0);
    MainWindow* parent;
    MLabel* communication = new MLabel(IDENTIFIER_MAIN,1,"沟通");
    MLabel* grade = new MLabel(IDENTIFIER_MAIN,4,"成绩查询");
    MLabel* profile = new MLabel(IDENTIFIER_MAIN,5,"我");
    MLabel* exit = new MLabel(IDENTIFIER_MAIN,0,"退出");
    QVector<MLabel*> container_client_main_list;

    /*communication page*/
    QVector<MLabel*> all_communication_list;
    QVector<MLabel*> communication_teacher_name_list;
    QVector<MLabel*> communication_student_name_list;
    scroll_label<QVector<MLabel*>>* communication_list_scroll;
    QVector<MLabel*> communication_display_page;
    QHash<int,QVector<MLabel*>> communication_content_log;


    /*ui area*/
     void show_client(MainWindow* parent); /*display the main page of this software*/
     void show_main_list(MainWindow* parent);

     /*ui prepared such as the initilization of these controls*/
    void init_main_list(MainWindow* parent);
/*logical process are --------------------------------------------------------------------------------------------------------------------------------------------------*/
     QTcpSocket* socket = nullptr;
     mthread* socket_thread = nullptr;
     QMutex* mutex_read;
     QQueue<message*> read_msgs;
     QQueue<message*> send_msgs;

    void connect_server();
    bool client_login(message msg);
    bool client_register(message msg);
    void send(message msg);
signals:
    void write_to_server(message* m);
public slots:
    void show_data();

};

#endif // CLIENT_H
