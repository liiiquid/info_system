#include "mlabel.h"
#include "ui_config.h"
#include <QDebug>
MLabel::MLabel(char identifier, int index, QString title):QLabel(title),index(index),identifier(identifier)
{

}
MLabel::MLabel(char identifier, int index):QLabel(),index(index),identifier(identifier)
{

}
void MLabel::mousePressEvent(QMouseEvent*)
{
    if(this->identifier == IDENTIFIER_MAIN)
        emit label_clicked(this->index);
    else if(this->identifier ==IDENTIFIER_MSG)
        emit msg_clicked(this->index);
    else if(this->identifier == IDENTIFIER_CONTACT)
        emit contact_clicked(this->index);
}



void MLabel::enterEvent(QEvent *)
{
    isenter = 1;
    update();
}

void MLabel::leaveEvent(QEvent *)
{
    isenter = 0;
    update();
}

void MLabel::paintEvent(QPaintEvent *)
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

void MLabel::setId(QString newId)
{
    id = newId;
}
