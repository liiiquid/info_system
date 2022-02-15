#include "mlabel.h"
#include "ui_config.h"

MLabel::MLabel(char identifier, int index, QString title):QLabel(title),index(index),identifier(identifier)
{

}
MLabel::MLabel(char identifier, int index):QLabel(),index(index),identifier(identifier)
{

}
void MLabel::mousePressEvent(QMouseEvent*)
{
    if(this->identifier == IDENTIFIER_MAIN)
        emit label_clicked(this->index);
    else if(this->identifier ==IDENTIFIER_MSG)
        emit msg_clicked(this->index);
    else if(this->identifier == IDENTIFIER_INFO)
        emit info_clicked(this->index);
}


void MLabel::setId(QString newId)
{
    id = newId;
}
