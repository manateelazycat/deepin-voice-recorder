#include "edit_file_button_delegate.h"
#include <QIntValidator>
#include <QEvent>

#include "edit_file_button.h"

#include <QDebug>

EditFileButtonDelegate::EditFileButtonDelegate(QObject *parent) : QItemDelegate(parent)
{
}

QWidget *EditFileButtonDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Return null mean don't allow user edit.
    return  NULL;
}


void EditFileButtonDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    // QString value =index.model()->data(index, Qt::EditRole).toString();
    // EditFileButton *button = static_cast<EditFileButton*>(editor);
    // button->lengthLabel->setText(value);
}


void EditFileButtonDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    // EditFileButton *button = static_cast<EditFileButton*>(editor);
    // QString value = button->lengthLabel->text();
    // model->setData(index, value);
}


void EditFileButtonDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}

bool EditFileButtonDelegate::editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index)
{
    qDebug() << this << event->type() << index;
}
