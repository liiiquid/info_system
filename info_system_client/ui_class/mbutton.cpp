#include "mbutton.h"
#include <QDebug>
#include <QPainter>
MButton::MButton(int index,QWidget* parent):QPushButton(parent),index(index)
{
    setStyleSheet("QPushButton{color:purple; background-color:transparent; border: 1px solid black;}");
}

void MButton::mousePressEvent(QMouseEvent *e)
{
    emit mbtn_clicked(this->index);
}

void MButton::enterEvent(QEvent *)
{
    isenter = 1;
}

void MButton::leaveEvent(QEvent *)
{
    isenter = 0;
}

void MButton::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.setPen(QColor(0,0,0));
    if(isenter)
    {
        p.fillRect(0,0,this->width(),this->height(),QBrush(QColor(190 , 231 ,233)));
    }
    else{
         p.fillRect(0,0,this->width(),this->height(),QBrush(QColor(250,250,250)));
    }
    p.drawText(0,0,this->width(),this->height(),Qt::AlignCenter,this->text());
}
