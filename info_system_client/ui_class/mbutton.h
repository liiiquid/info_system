#ifndef MBUTTON_H
#define MBUTTON_H
#include <QPushButton>


class MButton:public QPushButton
{
    Q_OBJECT
public:
    MButton(int index,QWidget *parent = nullptr);
    void mousePressEvent(QMouseEvent *e);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    void paintEvent(QPaintEvent*);
public:
    int index;
    int isenter = 0;
signals:
    void mbtn_clicked(int);
};

#endif // MBUTTON_H
