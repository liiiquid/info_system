#ifndef TEXT_EDIT_H
#define TEXT_EDIT_H
#include <QTextEdit>
#include <QLineEdit>
class text_edit : public QLineEdit
{
    Q_OBJECT
public:
    text_edit(QWidget* parent);
    void focusOutEvent(QFocusEvent*);
signals:
    void edit_finished();
};

#endif // TEXT_EDIT_H

