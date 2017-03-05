#ifndef ICONDELEGATE_H
#define ICONDELEGATE_H

#include <QItemDelegate>

class IconDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit IconDelegate(QObject *parent = 0);

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif
