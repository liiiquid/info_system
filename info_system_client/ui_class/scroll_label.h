#ifndef SCROLL_LABEL_H
#define SCROLL_LABEL_H
#include <QLabel>
#include <QWheelEvent>
#include "head_label.h"

class client;
class scroll_label : public QLabel
{
    Q_OBJECT
public:
    scroll_label(client* cli,QWidget* parent);
    void wheelEvent(QWheelEvent* ev);
    void paintEvent(QPaintEvent*);
    void add_first_control(head_label* first_control);
    void add_second_control(head_label* first_control, head_label* second_control);
    void clear_scroll_list();
    void set_location(head_label* ellm);

    void add_info_elem(head_label* ellm);
     QVector<head_label*> info_list_controls;

     void set_scroll_page(QVector<QLabel*>* ellm);
    QVector<QLabel*>* info_page;

    int is_info = 0;/*0 - contact list, 1 - info list, 2 - info page, mainly used to recognize how to scroll*/
    int is_scroll = 0;
    int y_start = 1;
    int total_len;
    int length;
    client* cli;
    QVector<head_label*> fir_level_controls;
    QHash<head_label*,QVector<head_label*>*>  sec_level_controls;
    int u;
signals:
    void contact_elem_clicked(head_label* ellm);
public slots:
    void show_hide_elemes(head_label* ellm);
    void update_list(head_label* ellm);
};

#endif // SCROLL_LABEL_H
