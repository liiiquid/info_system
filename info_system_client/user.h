#ifndef USER_H
#define USER_H
#include <QObject>
#include "ui_class/head_label.h"
class MainWindow;
class user : public QObject
{
    Q_OBJECT
public:
    user();
    user(int id, QString name, int type);
    user(int id, QString name, int type,MainWindow* parent,int x,int y, int width, int height);
    void user_show();
    void user_set_img(QImage* img);
    void user_set_text(QString text, QString text_assistant = "");
    int id;
    QString name;
    int type;
    int x; int y;int width,height;
    head_label* label;
    MainWindow* parent;
};

#endif // USER_H
