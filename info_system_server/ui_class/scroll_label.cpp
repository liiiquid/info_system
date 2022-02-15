#include "scroll_label.h"
#include <QDebug>
scroll_label::scroll_label(QWidget* parent,QVector<QVector<label_editable*>>* label) : QLabel(parent)
{
    this->m_label = label;
    for(int i = 0; i < this->m_label->size();i++)
    {
        for(int j = 0; j < this->m_label->at(0).size();j++)
        {
            this->m_label->at(i)[j]->setParent(this);
        }
    }
}

void scroll_label::wheelEvent(QWheelEvent* ev)
{
    int delta = (ev->delta() / 5);



    for(int i = 0; i < m_label->size();i++)
    {
        for(int j = 0; j < m_label->at(i).size();j++)
        {
            m_label->at(i)[j]->move(m_label->at(i)[j]->x(),m_label->at(i)[j]->y() + delta);
        }
    }

    this->update();
}
