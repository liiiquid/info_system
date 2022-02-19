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
    QImage* img;
    int id;
    int is_contact = 1;
    int is_enter = 0;
    user* parent;
signals:
    void contact_clicked(int id);
    void info_clicked(int id);
};

#endif // HEAD_LABEL_H
