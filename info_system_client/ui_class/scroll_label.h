#ifndef SCROLL_LABEL_H
#define SCROLL_LABEL_H
#include <QLabel>
#include <QWheelEvent>
#include "label_editable.h"

template <class T>
class scroll_label : public QLabel
{
public:
    scroll_label(QWidget* parent,QVector<QVector<T>>* label);
    QVector<QVector<T>>* m_label;
    void wheelEvent(QWheelEvent* ev);
};

#endif // SCROLL_LABEL_H
