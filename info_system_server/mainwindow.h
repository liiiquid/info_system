#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPainter>
#include "server.h"
#include "ui_class/mbutton.h"
#include "ui_class/mlabel.h"
#include "ui_class/mlineedit.h"
#include "message.h"


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
    bool admin_login(message msg);
    bool admin_register(message msg);

/*server ui interface*/
    void goto_server();
/*control the server, if the server is start, then begin to listen in a port, or stop listenning*/
    bool server_start();
    bool server_end();

/*server object!!!*/
public:
   server server;

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

/*ui object*/
#define btn_size 3 /*represent the num of btns in main window*/
#define label_size 2 /*represent the num of labels in main window*/
#define edit_size 2 /*represent the num of edits in main window*/
private:
    Ui::MainWindow *ui;
    MButton* btns[btn_size] ={new MButton(0),new MButton(1),new MButton(2)}; /* 0 - login,1 - register, 2 - exit*/
    QString btn_text[btn_size] =
                                {"登陆","注册","退出"};

    MLabel* labels[label_size] = {new MLabel('q',0),new MLabel('q',1)};/*0 - id, 1 - password*/
    QString label_text[label_size] =
                                    {"账号","密码"};
    MLineEdit* edits[edit_size] = {new MLineEdit(0),new MLineEdit(1)};/*0 - id, 1 - password*/
    QString edits_text[edit_size] =
                                    {"请输入账号","请输入密码"};
    QImage* bkimg;
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
    void login_process();
    void register_process();
    void exit_process();
    message get_edit_info();

};


#endif // MAINWINDOW_H
