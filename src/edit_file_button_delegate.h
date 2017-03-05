#ifndef EDITFILEBUTTONDELEGATE_H
#define EDITFILEBUTTONDELEGATE_H

#include <QItemDelegate>

class EditFileButtonDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit EditFileButtonDelegate(QObject *parent = 0);

protected:
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
    void updateEditorGeometry(QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    bool editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index);

signals:

public slots:

};

#endif
