#include "mbutton.h"
#include <QDebug>
MButton::MButton(int index,QWidget* parent):QPushButton(parent),index(index)
{
    setStyleSheet("QPushButton{color:purple; background-color:transparent; border: 1px solid black;}");
}

void MButton::mousePressEvent(QMouseEvent *e)
{
    emit mbtn_clicked(this->index);
}
