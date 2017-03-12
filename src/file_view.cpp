#include "file_view.h"
#include <QFileInfoList>
#include <QDir>
#include <QMouseEvent>

#include <QLabel>
#include <QProcess>
#include <QListWidgetItem>

#include <QDebug>

#include "utils.h"
#include "file_item.h"

FileView::FileView(QWidget *parent) : QListWidget(parent)
{
    setFixedSize(433, 250);

    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    connect(this, SIGNAL(rightClick(QPoint)), this,SLOT(onRightClick(QPoint)));

    rightMenu = new QMenu();
    renameAction = new QAction(tr("Rename"), this);
    connect(renameAction, &QAction::triggered, this, &FileView::renameItem);
    displayAction = new QAction(tr("Display in file manager"), this);
    connect(displayAction, &QAction::triggered, this, &FileView::displayItem);
    deleteAction = new QAction(tr("Delete"), this);
    connect(deleteAction, &QAction::triggered, this, &FileView::deleteItem);
    rightMenu->addAction(renameAction);
    rightMenu->addAction(displayAction);
    rightMenu->addAction(deleteAction);

    QFileInfoList fileInfoList = Utils::getRecordingFileinfos();

    foreach (auto fileInfo, fileInfoList) {
        FileItem *fileItem = new FileItem();
        fileItem->setFileInfo(fileInfo);
        connect(fileItem, SIGNAL(play()), this, SLOT(handlePlay()));
        connect(fileItem, SIGNAL(pause()), this, SLOT(handlePause()));
        connect(fileItem, SIGNAL(resume()), this, SLOT(handleResume()));
        connect(fileItem, SIGNAL(stop()), this, SLOT(handleStop()));

        addItem(fileItem->getItem());
        fileItem->getItem()->setSizeHint(QSize(width(), 60));
        setItemWidget(fileItem->getItem(), fileItem);
    }

    connect(this, &QListWidget::currentItemChanged, this, &FileView::handleCurentItemChanged);
    connect(this, &QListWidget::itemClicked, this, &FileView::handleItemClicked);
}

void FileView::mousePressEvent(QMouseEvent *event)
{
    QListWidget::mousePressEvent(event);

    if(event->button() == Qt::RightButton){
        emit rightClick(event->pos());
    }
}

void FileView::onRightClick(QPoint pos)
{
    rightSelectItem = itemAt(pos);

    rightMenu->exec(this->mapToGlobal(pos));
}

void FileView::renameItem()
{
    if (rightSelectItem != 0) {
        FileItem *fileItem = static_cast<FileItem *>(itemWidget(rightSelectItem));
        fileItem->switchStatus(FileItem::STATUS_RENAME);
    }
}

void FileView::displayItem()
{
    if (rightSelectItem != 0) {
        FileItem *fileItem = static_cast<FileItem *>(itemWidget(rightSelectItem));
        auto dirUrl = QUrl::fromLocalFile(fileItem->getFileInfo().absoluteDir().absolutePath());
        QFileInfo ddefilemanger("/usr/bin/dde-file-manager");
        if (ddefilemanger.exists()) {
            auto dirFile = QUrl::fromLocalFile(fileItem->getFileInfo().absoluteFilePath());
            auto url = QString("%1?selectUrl=%2").arg(dirUrl.toString()).arg(dirFile.toString());
            QProcess::startDetached("dde-file-manager" , QStringList() << url);
        } else {
            QProcess::startDetached("gvfs-open" , QStringList() << dirUrl.toString());
        }
    }
}

void FileView::deleteItem()
{
    if (rightSelectItem != 0) {
        FileItem *fileItem = static_cast<FileItem *>(itemWidget(rightSelectItem));
        emit stop(fileItem->getRecodingFilepath());

        delete takeItem(row(rightSelectItem));
    }
}

void FileView::handleCurentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    // Restore previous item status.
    if (previous != 0) {
        FileItem *fileItem = static_cast<FileItem *>(itemWidget(previous));
        fileItem->switchStatus(FileItem::STATUS_NORMAL);
    }

    // Update current item status.
    if (current != 0) {
        if (currentWidgetItem != 0) {
            FileItem *fileItem = static_cast<FileItem *>(itemWidget(current));
            fileItem->switchStatus(FileItem::STATUS_PLAY);
        }

        currentWidgetItem = current;
    }
}

void FileView::handleItemClicked(QListWidgetItem *item)
{
    setCurrentItem(item);

    // NOTE: At last need change current item status, because click same item won't triggered currentItemChanged signal.
    FileItem *fileItem = static_cast<FileItem *>(itemWidget(item));
    fileItem->switchStatus(FileItem::STATUS_PLAY);
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
    FileItem *fileItem = static_cast<FileItem *>(itemWidget(currentWidgetItem));
    if (filepath == fileItem->getRecodingFilepath()) {
        fileItem->switchStatus(FileItem::STATUS_NORMAL);
    }
}

void FileView::selectItemWithPath(QString path)
{
    for(int i = 0; i < count(); i++) {
        QListWidgetItem* matchItem = item(i);
        FileItem *fileItem = static_cast<FileItem *>(itemWidget(matchItem));
        
        if (fileItem->getRecodingFilepath() == path) {
            setCurrentItem(matchItem);
            break;
        }
    }
}
