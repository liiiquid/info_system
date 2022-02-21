#include "label_editable.h"
#include <QDebug>
label_editable::label_editable(QWidget *parent) : QLabel(parent)
{

}
void label_editable::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(store) {text_origin = this->text(); store = false;}
    if(line_edit == nullptr)
        line_edit = new text_edit(this);
    QObject::connect(line_edit,&text_edit::edit_finished,this,&label_editable::get_edit_text);
    line_edit->setFixedSize(this->width(),this->height());
    line_edit->move(0,0);
    line_edit->setText(this->text());
    line_edit->show();
    line_edit->setFocus();
}


void label_editable::get_edit_text()
{
     this->setText(line_edit->text());
    if(this->text() != text_origin) edited = true;
}
