#include "mbutton.h"
#include <QDebug>
MButton::MButton(int index,QWidget* parent):QPushButton(parent),index(index)
{

}

void MButton::mousePressEvent(QMouseEvent *e)
{
    emit mbtn_clicked(this->index);
}
