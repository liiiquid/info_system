#include "select_label.h"
#include <QPainter>
select_label::select_label() : QLabel()
{

}

void select_label::mousePressEvent(QMouseEvent *ev)
{
    if(isselect == 0) isselect = 1;
    else isselect = 0;
    this->update();
}

void select_label::paintEvent(QPaintEvent* )
{
    QPainter p;
    QBrush brush;
    if(isselect)
    {
        p.drawRect(0,0,this->width(),this->height());
    }
}
