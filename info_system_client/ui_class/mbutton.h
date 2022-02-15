#ifndef MBUTTON_H
#define MBUTTON_H
#include <QPushButton>


class MButton:public QPushButton
{
    Q_OBJECT
public:
    MButton(int index,QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
public:
    int index;
signals:
    void mbtn_clicked(int);
};

#endif // MBUTTON_H
