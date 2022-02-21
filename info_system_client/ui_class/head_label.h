#ifndef HEAD_LABEL_H
#define HEAD_LABEL_H

#include <QLabel>
#include <QImage>
class user;
class head_label : public QLabel
{
    Q_OBJECT
public:
    head_label(user* user,int is_contact = 1);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);
    void enterEvent(QEvent*);
    void leaveEvent(QEvent*);
    QString text_assistant;
    QImage* img = nullptr;
    int id = -1;
    int is_contact = 1;
    int is_enter = 0;
    int is_level = 0;
    int is_get_server = 0;
    int is_open = 0;
    int unread = 0;
    QString name;
    user* parent = nullptr;
signals:
    void contact_clicked(head_label* ellm);
    void info_clicked(head_label* ellm);
};

#endif // HEAD_LABEL_H
