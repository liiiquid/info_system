#ifndef USER_H
#define USER_H
#include <QObject>

class user : public QObject
{
    Q_OBJECT
public:
    user();
    user(int id, QString name, int type);
    int id;
    QString name;
    int type;
};

#endif // USER_H
