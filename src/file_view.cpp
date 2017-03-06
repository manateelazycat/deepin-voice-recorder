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
        QListWidgetItem *item = new QListWidgetItem();
        FileItem *fileItem = new FileItem();
        fileItem->setFileInfo(fileInfo);

        addItem(item);
        item->setSizeHint(QSize(433, 60));
        setItemWidget(item, fileItem);
    }

    setFixedSize(433, 250);
}
