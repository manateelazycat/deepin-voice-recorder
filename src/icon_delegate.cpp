#include "icon_delegate.h"
#include <QLineEdit>
#include <QIntValidator>
#include <QApplication>

#include <QDebug>

IconDelegate::IconDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget* IconDelegate::createEditor(QWidget*parent, const QStyleOptionViewItem &option, const QModelIndex &index)  const
{
    // Return null mean don't allow user edit.
    return  NULL;
}

void  IconDelegate::paint(QPainter *painter,  const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QPixmap pixmap = QPixmap(index.data().toString());
    qApp->style()->drawItemPixmap(painter, option.rect,  Qt::AlignCenter, QPixmap(pixmap));
}
