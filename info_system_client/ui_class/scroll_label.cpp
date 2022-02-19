#include "scroll_label.h"
#include <QDebug>
#include <QPainter>

scroll_label::scroll_label(QWidget* parent,QVector<QVector<head_label*>>* label) : QLabel(parent)
{
    this->m_label_2 = label;
    for(int i = 0; i < this->m_label_2->size();i++)
    {
        for(int j = 0; j < this->m_label_2->at(0).size();j++)
        {
            this->m_label_2->at(i)[j]->setParent(this);
        }
    }
    total_len = this->m_label_2->size() * this->m_label_2->at(0)[0]->height();
     length = (qreal)this->height() * (qreal)this->height() / total_len;
    u = (double)total_len /(double) this->height();
    is_one = 0;
}


scroll_label::scroll_label(QWidget* parent, QVector<head_label*>* label):QLabel(parent)
{
    is_one = 1;
    this->m_label_1 = label;
    for(int i = 0; i  <m_label_1->size();i++)
    {
        this->m_label_1->at(i)->setParent(this);
    }
    total_len = this->m_label_1->size() * this->m_label_1->at(0)->height();
     length = (qreal)this->height() * (qreal)this->height() / total_len;
     u = (double)total_len /(double) this->height();
}


void scroll_label::paintEvent(QPaintEvent*)
{
    if(is_scroll)
    {

        QPainter p(this);
        p.fillRect(0,0,this->width(),this->height(),QColor(250,250,250));
        QPen pen;
        pen.setColor(QColor(0,0,0));
        pen.setWidth(5);
        p.setPen(pen);
        int x = this->width() - 10;
        p.drawLine(QPointF(x,y_start),QPointF(x,y_start + length));
    }
}


void scroll_label::wheelEvent(QWheelEvent* ev)
{
    int delta = (ev->delta() / 5);

    int dy = delta *u;
    if(!is_one)
    {
        int delta = ev->delta() / 12;
        y_start -= delta;
        for(int i = 0; i < this->m_label_2->size();i++)
        {
            for(int j  =0 ;j < this->m_label_2->at(i).size();j++)
            {
                this->m_label_2->at(i)[j]->move(m_label_2->at(i)[j]->x(), m_label_2->at(i)[j]->y() + dy);
            }
        }
    }else{
        for(int i = 0; i < m_label_1->size();i++ )
        {
            m_label_1->at(i)->move(m_label_1->at(i)->x(),m_label_1->at(i)->y() + dy);
        }
    }

    is_scroll = 1;
    this->update();
}


void scroll_label::setAll()
{
    for(int i = 0; i  <m_label_1->size();i++)
    {
        this->m_label_1->at(i)->setParent(this);
    }
}

void scroll_label::setLocation_1()
 {


 }
