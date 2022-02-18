#ifndef M_SWITCH_LABEL_H
#define M_SWITCH_LABEL_H


#include <QLabel>
class m_switch_label : public QLabel
{
public:
    m_switch_label();
    void paintEvent(QPaintEvent*);
};

#endif // M_SWITCH_LABEL_H
