#include "message_serialization.h"

message_serialization::message_serialization()
{

}
QString message_serialization::int2str(int n)
{
    QString res("");
    if(n == 0) return QString("0");
    char str[10];
    int i = 0;
    while(n)
    {
        str[i] = n % 10 + '0';
        n /= 10;
        i++;
    }
    i -=1;
    while(i >= 0)
    {res += str[i];i--;}
    return res;
}
int message_serialization::str2int(QString str)
{
    int res = 0;
    for(int i = 0;i < str.length(); i++)
    {
        QChar c = str.at(i);
        res = res*10 + c.unicode() - '0';
    }
    return res;
}
QVector<QString> message_serialization::split(QString str, QString regular)
{
    QVector<QString> res;
    int reg_len = regular.length();
    int len = str.length();
    int j = 0;int count = 0;
    bool matched = false;
    QString tmp = "";
    for(int i = 0; i < len; i++)
    {
        if(str[i] != regular[j])
        {tmp += str[i];}
        else{
            count = i;
            while(str[i] == regular[j])
            {
                i++;j++;if(j == reg_len){matched = true;break;}
            }
            if(matched)
            {
                res.push_back(tmp);tmp = "";matched = false;
            }else{
               for(;count <= i;count++) tmp += str[count];
            }
            j = 0;
            i--;
        }
    }
    if(tmp != "")
    {    res.push_back(tmp);}

    return res;
}


/*serialize message to utf-8 encode*/
QByteArray message_serialization::serialize(message* msg)
{
    QByteArray sended = "";
    sended += int2str((*msg).type);
    sended += SERIAL_FMT;
    sended += int2str((*msg).sender);
    sended += SERIAL_FMT;
    sended += int2str((*msg).send_type);
    sended += SERIAL_FMT;
    sended += int2str((*msg).receiver);
    sended += SERIAL_FMT;
    sended += int2str((*msg).receiver_type);
    sended += SERIAL_FMT;
    sended += ((*msg).content.toUtf8()).constData();
    return sended;
}

/*unserialize uft-8 encode to message format*/
message* message_serialization::unserialize(QString str)
{
    message* msg = new message();
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QVector<QString> strs = split(str,QString(SERIAL_FMT));
    if(strs.size() != NUM_ATTRIBUTE)
    {qDebug() << "message exception...  message_serialization() ";return new message(0,0,0,0,0,str);}
    msg->type = str2int(strs[0]);
    msg->sender = str2int(strs[1]);
    msg->send_type = str2int(strs[2]);
    msg->receiver = str2int(strs[3]);
    msg->receiver_type = str2int(strs[4]);
    msg->content = QString(codec->fromUnicode(strs[5]));
    return msg;
}

QVector<QString> message_serialization::analysis_content(QString content)
{
    QVector<QString> res;
    QString str = "";
    for(int i = 0; i < content.length();i++)
    {
        QCharRef c = content[i];
        if(c != SEG_ONE_LINE)
            str+=content[i];
        else
        {
            res.push_back(str);
            str = "";
            if(content[i+1] == SEG_SWT_LINE) i++;
        }
    }
    return res;
}

QVector<QVector<QString>> message_serialization::analysis_content_2(QString content)
{
    QVector<QVector<QString>> ret;
    QVector<QString> res;
    QString str = "";
    for(int i = 0; i < content.length();i++)
    {
        QCharRef c = content[i];
        if(c != SEG_ONE_LINE)
            str+=content[i];
        else
        {
            res.push_back(str);
            str = "";
            if(content[i+1] == SEG_SWT_LINE){ret.push_back(res);  res.clear(); i++;}
        }
    }
    return ret;
}
