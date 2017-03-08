#include "file_view.h"
#include <QFileInfoList>
#include <QDir>

#include <QLabel>
#include <QListWidgetItem>

#include <QDebug>

#include "utils.h"
#include "file_item.h"

FileView::FileView(QListWidget *parent) : QListWidget(parent)
{
    clickedRenameButton = false;
        
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QStringList filters;
    filters << "*.wav";
    QFileInfoList fileInfoList = QDir("/home/andy/Music/Deepin Voice Recorder").entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

    foreach (auto fileInfo, fileInfoList) {
        FileItem *fileItem = new FileItem();
        fileItem->setFileInfo(fileInfo);
        connect(fileItem, SIGNAL(clickedRenameButton()), this, SLOT(handleClickedRenameButton()), Qt::QueuedConnection);

        addItem(fileItem->getItem());
        fileItem->getItem()->setSizeHint(QSize(433, 60));
        setItemWidget(fileItem->getItem(), fileItem);
    }

    setFixedSize(433, 250);
    
    connect(this, &QListWidget::currentItemChanged, this, &FileView::handleCurentItemChanged);
}

void FileView::handleCurentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    // Restore previous item status.
    if (previous != 0) {
        FileItem *widget = static_cast<FileItem *>(itemWidget(previous));
        widget->switchStatus(FileItem::STATUS_NORMAL);
    }
    
    // Update current item status.
    if (current != 0) {
        FileItem *widget = static_cast<FileItem *>(itemWidget(current));
        if (clickedRenameButton) {
            widget->switchStatus(FileItem::STATUS_RENAME);
            clickedRenameButton = false;
        } else {
            widget->switchStatus(FileItem::STATUS_PLAY);
        }
        
        currentWidgetItem = current;
    }
}

void FileView::handleClickedRenameButton()
{
    clickedRenameButton = true;
    setCurrentItem(((FileItem*) sender())->getItem());
}
