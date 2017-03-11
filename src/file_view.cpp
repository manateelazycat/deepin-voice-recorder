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
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QStringList filters;
    filters << "*.wav";
    QFileInfoList fileInfoList = QDir("/home/andy/Music/Deepin Voice Recorder").entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

    foreach (auto fileInfo, fileInfoList) {
        FileItem *fileItem = new FileItem();
        fileItem->setFileInfo(fileInfo);
        connect(fileItem, SIGNAL(clickedRenameButton()), this, SLOT(handleClickedRenameButton()));
        connect(fileItem, SIGNAL(play()), this, SLOT(handlePlay()));
        connect(fileItem, SIGNAL(pause()), this, SLOT(handlePause()));
        connect(fileItem, SIGNAL(resume()), this, SLOT(handleResume()));
        connect(fileItem, SIGNAL(stop()), this, SLOT(handleStop()));

        addItem(fileItem->getItem());
        fileItem->getItem()->setSizeHint(QSize(433, 60));
        setItemWidget(fileItem->getItem(), fileItem);
    }

    setFixedSize(433, 250);

    connect(this, &QListWidget::currentItemChanged, this, &FileView::handleCurentItemChanged);
    connect(this, &QListWidget::itemClicked, this, &FileView::handleItemClicked);
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
        widget->switchStatus(FileItem::STATUS_PLAY);

        currentWidgetItem = current;
    }
}

void FileView::handleItemClicked(QListWidgetItem *item)
{
    setCurrentItem(item);
}

void FileView::handleClickedRenameButton()
{
    setCurrentItem(((FileItem*) sender())->getItem());

    FileItem *widget = static_cast<FileItem *>(itemWidget(((FileItem*) sender())->getItem()));
    widget->switchStatus(FileItem::STATUS_RENAME);
}

void FileView::handlePlay()
{
    emit play(((FileItem*) sender())->getRecodingFilepath());
}

void FileView::handlePause()
{
    emit pause(((FileItem*) sender())->getRecodingFilepath());
}

void FileView::handleResume()
{
    emit resume(((FileItem*) sender())->getRecodingFilepath());
}

void FileView::handleStop()
{
    emit stop(((FileItem*) sender())->getRecodingFilepath());
}

void FileView::handlePlayFinish(QString filepath)
{
    FileItem *widget = static_cast<FileItem *>(itemWidget(currentWidgetItem));
    if (filepath == widget->getRecodingFilepath()) {
        widget->switchStatus(FileItem::STATUS_NORMAL);
    }
}
