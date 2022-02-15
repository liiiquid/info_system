#ifndef MLINEEDIT_H
#define MLINEEDIT_H

#include <QLineEdit>

class MLineEdit : public QLineEdit
{
public:
    MLineEdit(int index);
public:
    int index;
};

#endif // MLINEEDIT_H
