#ifndef SELECT_LABEL_H
#define SELECT_LABEL_H
#include <QLabel>

class select_label : public QLabel
{
public:
    select_label();
    void mousePressEvent(QMouseEvent* ev);
    void paintEvent(QPaintEvent* ev);
    int isselect = 0;
};

#endif // SELECT_LABEL_H
