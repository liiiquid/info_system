#ifndef M_EVENT_H
#define M_EVENT_H
#include <QEvent>

#include "../message.h"
class m_event : public QEvent
{
public:
    m_event(QEvent::Type type);
    message mg;
};

#endif // M_EVENT_H
