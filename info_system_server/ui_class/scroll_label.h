#ifndef SCROLL_LABEL_H
#define SCROLL_LABEL_H
#include <QLabel>
#include <QWheelEvent>
#include "label_editable.h"
class scroll_label : public QLabel
{
public:
    scroll_label(QWidget* parent,QVector<QVector<label_editable*>>* label);
    QVector<QVector<label_editable*>>* m_label;
    void wheelEvent(QWheelEvent* ev);
};

#endif // SCROLL_LABEL_H
