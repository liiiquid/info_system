#ifndef MLABEL_H
#define MLABEL_H

#include <QLabel>

class MLabel : public QLabel
{
    Q_OBJECT
public:
    MLabel(char identifier,int index,QString title);
    MLabel(char identifier,int index);
public:
    void mousePressEvent(QMouseEvent* e);
    void set_msg_type(int type);
public:
    int index;
    QString id; /*used to recognize the type of message*/
    QString identifier;
    void setId(QString newId);
signals:
    void label_clicked(int);
    void msg_clicked(int);
    void info_clicked(int);
};

#endif // MLABEL_H
