#ifndef M_THREAD_POOL_H
#define M_THREAD_POOL_H
#include <QThread>
#include <QList>
#include "mthread.h"
class m_thread_pool:public QObject
{
    Q_OBJECT
public:
    m_thread_pool(int size);
    bool start(mthread* thread);
    bool close_wait_for_all();
public slots:
    bool close_thread(mthread* thread);
public:
    int size;
    int curnum = 0;
    QList<mthread*> thread_all;

};

#endif // M_THREAD_POOL_H
