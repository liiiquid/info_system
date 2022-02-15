#include "scroll_label.h"
#include <QDebug>

template <class T>
scroll_label<T>::scroll_label(QWidget* parent,QVector<QVector<T>>* label) : QLabel(parent)
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

template<class T>
void scroll_label<T>::wheelEvent(QWheelEvent* ev)
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
