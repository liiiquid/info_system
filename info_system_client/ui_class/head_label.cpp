#include "head_label.h"
#include <QPainter>
#include "../user.h"
head_label::head_label(user *user, int is_contact) :  QLabel()
{
    this->parent = user;
    this->is_contact = is_contact;
}

void head_label::paintEvent(QPaintEvent* )
{
    QPainter p(this);
     if(is_enter)
     {
           p.fillRect(0,0,this->width(),this->height(),QBrush(QColor(190 , 231 ,233)));
     }else{
         p.fillRect(0,0,this->width(),this->height(),QBrush(QColor(250,250,250)));
     }
     if(this->img)
     {
         p.drawImage(QRect(0,0,50,50),*this->img);
     }
    p.setPen(QColor(0,0,0));
    if(is_contact)
    {
        p.drawText(QRect(50,10,200,50),Qt::AlignLeft,this->text());
    }else{
        p.drawText(QRect(50,0,200,25),Qt::AlignLeft,this->text());
        if(unread)
        {
            p.setPen(QColor(255,100,100));
        }
        p.drawText(QRect(50,25,200,25),Qt::AlignLeft,this->text_assistant);
    }
}

void head_label::mousePressEvent(QMouseEvent *)
{
    if(this->is_level == 1)/*if is merge*/
    {
        if(this->is_open == 1) this->is_open = 0;
        else this->is_open = 1;
    }
    if(is_contact) emit contact_clicked(this);
    else emit info_clicked(this);
    if(unread) unread = 0;
}

void head_label::enterEvent(QEvent*)
{
    is_enter = 1;
}

void head_label::leaveEvent(QEvent *)
{
    is_enter = 0;
}

