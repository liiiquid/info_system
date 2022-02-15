#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QDebug>
#define NUM_ATTRIBUTE 6
#define NUM_TYPE 9
#define SEG_ONE_LINE '\t'
#define SEG_SWT_LINE '\n'
#define SERIAL_FMT '\r'
class message
{
public:
    static const QString type_table[NUM_TYPE];
    message();
    message(/*int size,*/int type, int sender, int send_type, int receiver, int receiver_type,QString content);
    message operator=(message msg)
    {
        this->type = msg.type;
        this->sender = msg.sender;
        this->send_type = msg.send_type;
        this->receiver = msg.receiver;
        this->receiver_type = msg.receiver_type;
        this->content = msg.content;
        return *this;
    }
    void print()
    {
        qDebug() << this->type;
        qDebug() <<this->sender;
        qDebug() <<this->send_type;
        qDebug() <<this->receiver;
        qDebug() <<this->receiver_type;
        qDebug() << this->content;
    }
public:
    /*int size;*/
    int type;
    int sender;
    int send_type;
    int receiver;
    int receiver_type;
    QString content;

};

#endif // MESSAGE_H
