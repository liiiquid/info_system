#ifndef M_EVENT_H
#define M_EVENT_H

#include <QEvent>
class m_event : public QEvent
{
public:
    m_event(QEvent::Type type);
};

#endif // M_EVENT_H
