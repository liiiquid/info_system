#ifndef LABEL_EDITABLE_H
#define LABEL_EDITABLE_H
#include <QLabel>
#include "text_edit.h"

class label_editable : public QLabel
{
public:
    void mouseDoubleClickEvent(QMouseEvent* event);
    text_edit* line_edit;
    label_editable(QWidget* parent = nullptr);
    QString text_origin;
    bool store = true;
    bool edited = false;
    bool added = false;
public slots:
    void get_edit_text();
};

#endif // LABEL_EDITABLE_H
