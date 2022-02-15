#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    this->set_bkimg();
    this->set_btn();
    this->set_laebl();
    this->set_edit();
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    setFixedSize(800,600);
    setWindowTitle("学生信息管理系统");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_bkimg()
{
    //this->bkimg = new QImage("");
}
void MainWindow::set_btn()
{

    this->btns[0]->move(300,360);
    this->btns[1]->move(300,360);
    this->btns[2]->move(400,360);
    this->btns[3]->move(400,360);
    this->btns[4]->move(350,390);
    this->btns[5]->move(0,0);
    this->btns[6]->move(0,0);
    for(int i = 0; i < btn_size;i++)
    {
        this->btns[i]->setText(btn_text[i]);
        this->btns[i]->setFixedSize(100,30);
        this->btns[i]->setParent(this);

        QObject::connect(btns[i],SIGNAL(mbtn_clicked(int)),this,SLOT(btn_process(int)));
        if(i == 0 || i == 2 || i == 4 || i == 5)
            login_page.push_back(this->btns[i]);
        else{
            register_page.push_back(this->btns[i]);
        }
    }
}
void MainWindow::set_laebl()
{
   this->labels[0]->move(290,250);
   this->labels[1]->move(290,PWD_LABEL_Y);
    for(int i = 0; i < label_size;i++)
    {
        this->labels[i]->setText(label_text[i]);
        this->labels[i]->setFixedSize(LABEL_WIDTH,30);
        this->labels[i]->setParent(this);
        this->labels[i]->show();
    }
}
void MainWindow::set_edit()
{
    this->edits[0]->move(320,250);
    this->edits[1]->move(320,PWD_EDIT_Y);
    for(int i = 0; i < edit_size;i++)
    {
        this->edits[i]->setPlaceholderText(edits_text[i]);
        this->edits[i]->setFixedSize(EDIT_WIDTH,30);
        this->edits[i]->setParent(this);
        this->edits[i]->show();
    }
}

void MainWindow::btn_process(int index)
{
    switch(index)
    {
    case 0:
        stu_login_process();
        break;
    case 1:
        stu_register_process();
        break;
    case 2:
        teacher_login_process();
        break;
    case 3:
        teacher_register_process();
        break;
    case 4:
        enter_register_page();
    case 5:
        exit_process();
        break;
    case 6:
        back_login_page();
        break;
    }
}

void MainWindow::stu_login_process()
{
    message msg = get_edit_info();
    msg.send_type = 2;
    if(this->client_login(msg)) /*repaint the window to the server interface!!!*/
     {
        goto_client();}
    else {}
}

void MainWindow::stu_register_process()
{
    message msg = get_edit_info();
    msg.send_type = 2;
    if(this->client_register(msg)) /*alert a dialog announce user the result*/
    {

    }else{}
}

void MainWindow::teacher_login_process()
{
    message msg = get_edit_info();
    msg.send_type = 1;
    if(this->client_login(msg)) /*repaint the window to the server interface!!!*/
     {
        goto_client();}
    else {}
}

void MainWindow::teacher_register_process()
{
    message msg = get_edit_info();
    msg.send_type = 1;
    if(this->client_register(msg)) /*alert a dialog announce user the result*/
    {

    }else{}
}

void MainWindow::enter_register_page()
{

}
bool MainWindow::client_login(message msg)
{
    return this->client.client_login(msg);
}
bool MainWindow::client_register(message msg)
{
    return this->client.client_register(msg);
}


void MainWindow::paintEvent(QPaintEvent *e)
{
    if(!state) return;
     QPainter p(this);
     p.fillRect(0,0,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT,QColor(MAIN_BGCOLOR(240)));
     p.drawLine(FST_LEVEL_PAGE_X,FST_LEVEL_PAGE_Y,FST_LEVEL_PAGE_X,MAIN_WINDOW_HEIGHT);/*segmentation 1*/
}


void MainWindow::goto_client()
{
    for(int i = 0; i < btn_size;i++)
    {
        btns[i]->hide();
    }
    for(int i = 0; i < label_size; i++)
    {
       labels[i]->hide();
    }
    for(int i = 0; i < edit_size;i++)
    {
        edits[i]->hide();
    }
    this->state = 1;
    this->client.show_client(this);
    update();

}

void MainWindow::mousePressEvent(QMouseEvent*)
{
    this->setFocus();
}

message MainWindow::get_edit_info()
{

    QString edit_text = "";
    for(int i = 0; i < edit_size;i++ )
    {
        edit_text += edits[i]->text();
        edit_text += '\t';
    }
    edit_text += '\n';  /*switch line identifier: \t\n*/
    /*after getting the information of text edit, set the edit to null*/
    for(int i = 0; i < edit_size;i++)
        edits[i]->setText("");
    return message(0,0,message_serialization::str2int(edits[0]->text()),0,0,edit_text);
}

void MainWindow::exit_process()
{
    for(int i = 0; i < btn_size;i++)
    {
        if(btns[i]) delete btns[i];
    }
    for(int i = 0; i < label_size; i++)
    {
       if(labels[i])delete labels[i];
    }
    for(int i = 0; i < edit_size;i++)
    {
        if(edits[i])delete edits[i];
    }
    QApplication::exit();
}

void MainWindow::back_login_page()
{

}





