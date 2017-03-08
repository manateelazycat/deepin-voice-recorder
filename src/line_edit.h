#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT
    
 public:
    LineEdit(QLineEdit *parent = 0);
    bool eventFilter(QObject *, QEvent *event);
    
signals:
    void pressEsc();
};

#endif
