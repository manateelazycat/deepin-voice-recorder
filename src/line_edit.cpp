#include "line_edit.h"
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>

LineEdit::LineEdit(QLineEdit *parent) : QLineEdit(parent)
{
    installEventFilter(this);  // add event filter
}

bool LineEdit::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent*>(event);
        
        if (keyEvent->key() == Qt::Key_Escape) {
            emit pressEsc();
        }
    }
    
    return false;
}
