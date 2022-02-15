#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include "client.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    int state = 0;
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent* e);
    void mousePressEvent(QMouseEvent*);
public:
/*when there are admins try to login or register, we will get their information of account, and verify the information in database via it*/
    bool client_login(message msg);
    bool client_register(message msg);

/*server ui interface*/
    void goto_client();
/*control the server, if the server is start, then begin to listen in a port, or stop listenning*/


/*client object!!!*/
public:
client client;
/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

/*ui object*/
#define btn_size 7 /*represent the num of btns in main window*/
#define label_size 2 /*represent the num of labels in main window*/
#define edit_size 2 /*represent the num of edits in main window*/
private:
    Ui::MainWindow *ui;
    MButton* btns[btn_size] ={new MButton(0),new MButton(1),new MButton(2),new MButton(3),new MButton(4),new MButton(5),new MButton(6)}; /* 0 - stu_login,1 - stu_register, 2 - teacher_login, 3 - teacher_register, 4 - register page, 5- register label,6 - exit*/
    QString btn_text[btn_size] ={"学生登陆","学生注册","教师登陆","教师注册","无账号?注册","退出","返回"};

    MLabel* labels[label_size] = {new MLabel('q',0),new MLabel('q',1)};/*0 - id, 1 - password*/
    QString label_text[label_size] =
                                    {"账号","密码"};
    MLineEdit* edits[edit_size] = {new MLineEdit(0),new MLineEdit(1)};/*0 - id, 1 - password*/
    QString edits_text[edit_size] =
                                    {"请输入账号","请输入密码"};
    QImage* bkimg;
    QVector<MButton*> login_page;
    QVector<MButton*> register_page;
public:
    /*the functions below there are all to set ui overview*/
    void set_bkimg();
    void set_btn();
    void set_laebl();
    void set_edit();
    void draw_server();

public slots:
    void btn_process(int);
/*slot process functions*/
public:

    void stu_login_process();
    void stu_register_process();
    void teacher_login_process();
    void teacher_register_process();
    void enter_register_page();
    void exit_process();
    void back_login_page();
    message get_edit_info();

};


#endif // MAINWINDOW_H
