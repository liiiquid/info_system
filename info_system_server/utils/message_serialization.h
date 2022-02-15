#ifndef MESSAGE_SERIALIZATION_H
#define MESSAGE_SERIALIZATION_H
#include "message.h"
#include <QTextCodec>
class message_serialization
{
public:
    message_serialization();
public:

    static QByteArray serialize(message* msg);
    static message* unserialize(QString str);
    static QVector<QString> analysis_content(QString content);
    static QVector<QVector<QString>> analysis_content_2(QString content); /*return the result with '\n'*/
    static QString int2str(int n);
    static int str2int(QString str);
    static QVector<QString> split(QString str, QString regular);
};

#endif // MESSAGE_SERIALIZATION_H
