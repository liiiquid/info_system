#include "user.h"
#include "mainwindow.h"
user::user()
{

}

user::user(int id, QString name, int type,MainWindow* parent,int x,int y, int width, int height)
{
    this->id = id;
    this->name = name;
    this->type = type;
    this->parent = parent;
    this->label = new head_label(this);
    label->move(x,y);
    label->setParent(this->parent);
    label->setText(this->name);
    label->setFixedSize(width,height);
    label->id = this->id;
}

user::user(int id, QString name, int type)
{
    this->id = id;
    this->name = name;
    this->type = type;
}

void user::user_show()
{
    label->show();
}

void user::user_set_img(QImage *img)
{
    label->img = img;
}

void user::user_set_text(QString text, QString text_assistant)
{
    this->label->setText(text);
    this->label->text_assistant = text_assistant;
}
