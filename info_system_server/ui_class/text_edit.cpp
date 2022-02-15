#include "text_edit.h"

text_edit::text_edit(QWidget* parent):QLineEdit(parent)
{

}

void text_edit::focusOutEvent(QFocusEvent *)
{
    this->hide();
    emit edit_finished();
}
