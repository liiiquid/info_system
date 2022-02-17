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
    else if(this->identifier == IDENTIFIER_INFO)
        emit info_clicked(this->index);
}



void MLabel::enterEvent(QEvent *)
{
    isenter = 1;
}

void MLabel::leaveEvent(QEvent *)
{
    isenter = 0;
}

void MLabel::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawText(2,10,this->width(),this->height(),Qt::AlignLeft,this->text());

    if(isenter)
    {
        p.drawRect(0,0,this->width(),this->height());
    }
    else{
        p.setPen(p.background().color());
        p.drawRect(0,0,this->width(),this->height());
    }

}


void MLabel::setId(QString newId)
{
    id = newId;
}
