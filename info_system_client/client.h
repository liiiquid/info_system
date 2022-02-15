#ifndef CLIENT_H
#define CLIENT_H
#include <QTcpSocket>
#include "message.h"
#include "utils/message_serialization.h"
#include "user.h"
#include "ui_class/mbutton.h"
#include "ui_class/mlabel.h"
#include "ui_class/mlineedit.h"
#include "ui_class/ui_config.h"
#include "ui_class/scroll_label.h"
class client : public QObject
{
    Q_OBJECT
public:
    client();
    ~client();
    user* cur_user;
    QWidget* parent;
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
     void show_client(QWidget* parent); /*display the main page of this software*/
     void show_main_list(QWidget* parent);




     /*ui prepared such as the initilization of these controls*/
    void init_main_list(QWidget* parent);
/*logical process are --------------------------------------------------------------------------------------------------------------------------------------------------*/
     QTcpSocket socket;
     QVector<message> read_msgs;
     QVector<message> send_msgs;

    void connect_server();
    bool client_login(message msg);
    bool client_register(message msg);
    void send(message msg);

signals:
    void read_process_ok();

public slots:
    void read_process();
    void show_data();





};

#endif // CLIENT_H
