#include "file_item.h"
#include <QWidget>

#include "utils.h"

FileItem::FileItem(QWidget *parent) : QWidget(parent)
{
    layout = new QHBoxLayout();
    infoLayout = new QHBoxLayout();
    actionLayout = new QHBoxLayout();
    
    fileIcon = new QLabel();
    fileIcon->setPixmap(QPixmap::fromImage(QImage(Utils::getImagePath("file.png"))));
    fileName = new QLabel();
    lineEdit = new QLineEdit();
    lengthLabel = new QLabel("00:00");
    
    renameButton = new DImageButton(
        Utils::getImagePath("rename_normal.png"),
        Utils::getImagePath("rename_hover.png"),
        Utils::getImagePath("rename_press.png")
        );

    showNodeButton = new DImageButton(
        Utils::getImagePath("show_node_normal.png"),
        Utils::getImagePath("show_node_hover.png"),
        Utils::getImagePath("show_node_press.png")
        );

    playStartButton = new DImageButton(
        Utils::getImagePath("play_start_normal.png"),
        Utils::getImagePath("play_start_hover.png"),
        Utils::getImagePath("play_start_press.png")
        );

    playPauseButton = new DImageButton(
        Utils::getImagePath("play_pause_normal.png"),
        Utils::getImagePath("play_pause_hover.png"),
        Utils::getImagePath("play_pause_press.png")
        );

    playStopButton = new DImageButton(
        Utils::getImagePath("play_stop_normal.png"),
        Utils::getImagePath("play_stop_hover.png"),
        Utils::getImagePath("play_stop_press.png")
        );
    
    setLayout(layout);
    layout->addLayout(infoLayout);
    layout->addLayout(actionLayout);
    layout->setContentsMargins(28, 0, 40, 0);
    
    infoLayout->addWidget(fileIcon);
    infoLayout->setSpacing(16);
    infoLayout->addWidget(fileName);
    infoLayout->addWidget(renameButton);
    infoLayout->addSpacing(16);
    infoLayout->addStretch();
    
    actionLayout->addWidget(showNodeButton);
    actionLayout->setSpacing(4);
    actionLayout->addWidget(playStartButton);
}

void FileItem::setFileInfo(QFileInfo info)
{
    fileInfo = info;
    fileName->setText(info.baseName());
}
