#include "m_thread_pool.h"

m_thread_pool::m_thread_pool(int size)
{
    this->size = size;
}

bool m_thread_pool::start(mthread *thread)
{
    if(this->curnum > this->size) return false;
    this->thread_all.append(thread);
    this->curnum++;
    thread->start();
    return true;
}

bool m_thread_pool::close_thread(mthread* thread)
{
    if(thread == nullptr) return false;
    for(int i = 0; i< thread_all.size();i++)
    {
        if(this->thread_all.at(i) == thread)
        {
            thread->deleteLater();
            thread->wait();
            delete thread;  this->thread_all.removeAt(i);qDebug() << "thread delete successfully!";return true;
        }
    }
    return false;
}

bool m_thread_pool::close_wait_for_all()
{
    for(int i = 0; i < thread_all.size();i++)
    {
        if(this->thread_all.at(i)->isRunning())
        {
            thread_all.at(i)->terminate();
            thread_all.at(i)->deleteLater();
            qDebug() << "thread force deleted...";
        }
        delete this->thread_all.at(i); this->thread_all.removeAt(i);
    }
    return true;
}


