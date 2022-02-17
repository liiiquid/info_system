#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_class/ui_config.h"
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
    this->server.parent = this;
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

    this->btns[0]->move(LOGIN_BUTTON_X,LOGIN_BUTTON_Y);
    this->btns[1]->move(REG_BUTTON_X,REG_BUTTON_Y);
    this->btns[2]->move(EXIT_BUTTON_X,EXIT_BUTTON_Y);
    for(int i = 0; i < btn_size;i++)
    {
        this->btns[i]->setText(btn_text[i]);
        this->btns[i]->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
        this->btns[i]->setParent(this);
        this->btns[i]->show();
        QObject::connect(btns[i],SIGNAL(mbtn_clicked(int)),this,SLOT(btn_process(int)));
    }
}
void MainWindow::set_laebl()
{
   this->labels[0]->move(ID_LABEL_X,ID_LABEL_Y);
   this->labels[1]->move(PWD_LABEL_X,PWD_LABEL_Y);
    for(int i = 0; i < label_size;i++)
    {
        this->labels[i]->setText(label_text[i]);
        this->labels[i]->setFixedSize(LABEL_WIDTH,LABEL_HEIGHT);
        this->labels[i]->setParent(this);
        this->labels[i]->show();
    }
}
void MainWindow::set_edit()
{
    this->edits[0]->move(ID_EDIT_X,ID_EDIT_Y);
    this->edits[1]->move(PWD_EDIT_X,PWD_EDIT_Y);
    for(int i = 0; i < edit_size;i++)
    {
        this->edits[i]->setPlaceholderText(edits_text[i]);
        this->edits[i]->setFixedSize(EDIT_WIDTH,EDIT_HEIGHT);
        this->edits[i]->setParent(this);
        this->edits[i]->show();
    }
}


void MainWindow::btn_process(int index)
{
    switch(index)
    {
    case 0:
        login_process();
        break;
    case 1:
        register_process();
        break;
    case 2:
        exit_process();
        break;
    }
}
bool MainWindow::admin_login(message msg)
{
    return this->server.admin_login(msg);
}
bool MainWindow::admin_register(message msg)
{
    return this->server.admin_register(msg);
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    if(!state) return;
     QPainter p(this);
     p.fillRect(0,0,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT,QColor(MAIN_BGCOLOR(240)));
     p.drawLine(FST_LEVEL_PAGE_X,FST_LEVEL_PAGE_Y,FST_LEVEL_PAGE_X,MAIN_WINDOW_HEIGHT);/*segmentation 1*/
     if(this->server.ui_page == 0) /*server info page*/{

     }
     else if(this->server.ui_page == 1){
         p.drawLine(SEC_LEVEL_PAGE_X,SEC_LEVEL_MSG_PAGE_Y,SEC_LEVEL_PAGE_X,MAIN_WINDOW_HEIGHT); /*msg page*/
     }else if(this->server.ui_page == 13){
          if(this->server.is_info_page) p.drawLine(SEC_LEVEL_PAGE_X,SEC_LEVEL_INFO_PAGE_Y,SEC_LEVEL_PAGE_X,MAIN_WINDOW_HEIGHT); /*information page*/
          else p.drawLine(SEC_LEVEL_PAGE_X,SEC_LEVEL_MSG_PAGE_Y,SEC_LEVEL_PAGE_X,MAIN_WINDOW_HEIGHT); /*a line through whole the client area*/
     }else if(server.ui_page == 25)
     {
         p.drawLine(SEC_LEVEL_PAGE_X,SEC_LEVEL_MSG_PAGE_Y,SEC_LEVEL_PAGE_X,MAIN_WINDOW_HEIGHT); /*add page*/
         if(server.is_add_page_open)
         {
             p.drawLine(300,30,800,30);
             for(int i = 0; i < server.add_container_list.size();i++)
             {
                 label_editable* label_left = server.add_container_list[i][0];
                 label_editable* label_right = server.add_container_list[i].last();
                  if(label_left->y() + 60 >= 570 || label_left->y() + 60 <= 30) continue;
                 p.drawLine(label_left->x() + 300 ,label_left->y() + 60,label_right->x() + 500,label_right->y() + 60); /*draw the horizontal line for add controls in add list*/
             }
         }
     }
}


void MainWindow::goto_server()
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
    this->server.show_server(this);
    update();

}

void MainWindow::mousePressEvent(QMouseEvent*)
{
    this->setFocus();
}

bool MainWindow::server_start()
{
    return this->server.server_start();
}
bool MainWindow::server_end()
{
    return this->server.server_end();
}


void MainWindow::login_process()
{
    message msg = get_edit_info();
    if(this->admin_login(msg)) /*repaint the window to the server interface!!!*/
     {
        this->server.admins.append(new administrator(msg.content.split("\t")[0].toInt()));
        goto_server();}
    else {}
}

void MainWindow::register_process()
{
    message msg = get_edit_info();
    if(this->admin_register(msg)) /*alert a dialog announce user the result*/
    {

    }else{}
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
    return message(0,0,0,0,0,edit_text);
}

void MainWindow::exit_process()
{
    for(int i = 0; i < btn_size;i++)
    {
        delete btns[i];
    }
    for(int i = 0; i < label_size; i++)
    {
       delete labels[i];
    }
    for(int i = 0; i < edit_size;i++)
    {
        delete edits[i];
    }
    QApplication::exit();
}




