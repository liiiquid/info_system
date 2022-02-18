#include "m_switch_label.h"
#include <QPainter>
m_switch_label::m_switch_label() : QLabel()
{

}

void split(QString str, QVector<QString>& strs,int length)
{
    for(int i = 0; i < str.length();i+=length)
    {
        QString text_seg;
        for(int j = i; j < str.length() && j <= i+length;j++)
        {
            text_seg += str[j];
        }
        text_seg += '\n';
        strs.push_back(text_seg);
    }
}
  void m_switch_label::paintEvent(QPaintEvent *e)
{
        QPainter p(this);
        QVector<QString> strs; split(this->text(),strs,this->width() / 15);
        for(int i = 0; i < strs.size();i++)
        {
            p.drawText(5,i*15,this->width(),15,Qt::AlignLeft,strs[i]);
        }
}
