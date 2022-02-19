#ifndef SCROLL_LABEL_H
#define SCROLL_LABEL_H
#include <QLabel>
#include <QWheelEvent>
#include "head_label.h"


class scroll_label : public QLabel
{
public:
    scroll_label(QWidget* parent,QVector<QVector<head_label*>>* label_2);
    scroll_label(QWidget* parent, QVector<head_label*>* label_1);
    QVector<QVector<head_label*>>* m_label_2 = nullptr;
    QVector<head_label*>* m_label_1 = nullptr;
    void wheelEvent(QWheelEvent* ev);
    void paintEvent(QPaintEvent*);
    void setAll();
     void setLocation_1();
    QVector<head_label*> controls;
    int is_one = 0;
    int is_scroll = 0;
    int y_start = 1;
    int total_len;
    int length;
    int is_teacher_open = 0;
    int is_class_open = 0;
    int is_student_open = 0;
    qreal u;
};

#endif // SCROLL_LABEL_H
