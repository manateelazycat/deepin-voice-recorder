#include "file_item.h"
#include <QWidget>
#include <QTimer>

#include <QDir>

#include <QDebug>

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif

const int FileItem::STATUS_NORMAL = 0;
const int FileItem::STATUS_RENAME = 1;
const int FileItem::STATUS_PLAY = 2;
const int FileItem::STATUS_PLAY_PAUSE = 3;
const int FileItem::STATUS_PAUSE_PLAY = 4;

FileItem::FileItem(QWidget *parent) : QWidget(parent)
{
    item = new QListWidgetItem();

    layout = new QHBoxLayout();
    infoLayout = new QHBoxLayout();
    actionLayout = new QHBoxLayout();

    fileIcon = new QLabel();
    fileIcon->setPixmap(QPixmap::fromImage(QImage(Utils::getImagePath("file.png"))));
    fileName = new QLabel();
    lineEdit = new LineEdit();
    durationLabel = new QLabel("00:00");

    renameButton = new DImageButton(
        Utils::getImagePath("rename_normal.png"),
        Utils::getImagePath("rename_hover.png"),
        Utils::getImagePath("rename_press.png")
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

    pausePlayButton = new DImageButton(
        Utils::getImagePath("play_start_normal.png"),
        Utils::getImagePath("play_start_hover.png"),
        Utils::getImagePath("play_start_press.png")
        );

    pauseStopButton = new DImageButton(
        Utils::getImagePath("play_stop_normal.png"),
        Utils::getImagePath("play_stop_hover.png"),
        Utils::getImagePath("play_stop_press.png")
        );

    setLayout(layout);
    layout->addLayout(infoLayout);
    layout->addLayout(actionLayout);
    layout->setContentsMargins(28, 0, 40, 0);

    infoLayout->addWidget(fileIcon);

    fileDisplayContainer = new QWidget();
    fileDisplayLayout = new QHBoxLayout(fileDisplayContainer);
    fileDisplayLayout->setSpacing(16);
    fileDisplayLayout->addWidget(fileName);
    fileDisplayLayout->addWidget(renameButton);
    fileDisplayLayout->addSpacing(16);
    fileDisplayLayout->addStretch();

    fileRenameContainer = new QWidget();
    fileRenameLayout = new QHBoxLayout(fileRenameContainer);
    fileRenameLayout->setSpacing(16);
    fileRenameLayout->addWidget(lineEdit);
    fileRenameLayout->addSpacing(16);
    fileRenameLayout->addStretch();

    normalActionContainer = new QWidget();
    normalActionLayout = new QHBoxLayout(normalActionContainer);
    normalActionLayout->addWidget(durationLabel);

    playActionContainer = new QWidget();
    playActionLayout = new QHBoxLayout(playActionContainer);
    playActionLayout->addWidget(playStartButton);

    playPauseActionContainer = new QWidget();
    playPauseActionLayout = new QHBoxLayout(playPauseActionContainer);
    playPauseActionLayout->addWidget(playStopButton);
    playPauseActionLayout->setSpacing(4);
    playPauseActionLayout->addWidget(playPauseButton);

    pausePlayActionContainer = new QWidget();
    pausePlayActionLayout = new QHBoxLayout(pausePlayActionContainer);
    pausePlayActionLayout->addWidget(pauseStopButton);
    pausePlayActionLayout->setSpacing(4);
    pausePlayActionLayout->addWidget(pausePlayButton);

    switchStatus(STATUS_NORMAL);

    connect(renameButton, &DImageButton::clicked, [=] () {
            emit clickedRenameButton();
            
            lineEdit->setText(fileInfo.baseName());
            lineEdit->selectAll();
        });
    connect(lineEdit, &QLineEdit::editingFinished, [=] () {
            QString newFilename = lineEdit->text();
            if (newFilename != "") {
                QString oldFilepath = fileInfo.absoluteFilePath();
                QString newFilepath = fileInfo.absoluteDir().filePath(QString("%1.wav").arg(newFilename));
                
                fileInfo = QFileInfo(newFilepath);
                QFile(oldFilepath).rename(newFilepath);
                fileName->setText(newFilename);
            }
            
            switchStatus(STATUS_PLAY);
        });
    connect(lineEdit, &LineEdit::pressEsc, [=] () {
            switchStatus(STATUS_PLAY);
        });
    connect(playStartButton, &DImageButton::clicked, [=] () {
            switchStatus(STATUS_PLAY_PAUSE);
            
            emit play();
        });
    connect(playPauseButton, &DImageButton::clicked, [=] () {
            switchStatus(STATUS_PAUSE_PLAY);
            
            emit pause();
        });
    connect(pausePlayButton, &DImageButton::clicked, [=] () {
            switchStatus(STATUS_PLAY_PAUSE);
            
            emit resume();
        });
    connect(playStopButton, &DImageButton::clicked, [=] {
            switchStatus(STATUS_PLAY);
            
            emit stop();
        });
    connect(pauseStopButton, &DImageButton::clicked, [=] {
            switchStatus(STATUS_PLAY);
            
            emit stop();
        });
}

void FileItem::setFileInfo(QFileInfo info)
{
    fileInfo = info;
    fileName->setText(info.baseName());
    
    durationLabel->setText(Utils::formatMillisecond(getDuration()));
}

void FileItem::switchStatus(int status)
{
    switch(status) {
    case STATUS_NORMAL: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileDisplayContainer);
        renameButton->show();

        Utils::removeLayoutChild(actionLayout, 0);
        Utils::addLayoutWidget(actionLayout, normalActionContainer);
    }
        break;
    case STATUS_RENAME: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileRenameContainer);
        QTimer::singleShot(0, lineEdit, SLOT(setFocus()));
    }
        break;
    case STATUS_PLAY: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileDisplayContainer);
        renameButton->show();

        Utils::removeLayoutChild(actionLayout, 0);
        Utils::addLayoutWidget(actionLayout, playActionContainer);;
    }
        break;
    case STATUS_PLAY_PAUSE: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileDisplayContainer);
        renameButton->hide();

        Utils::removeLayoutChild(actionLayout, 0);
        Utils::addLayoutWidget(actionLayout, playPauseActionContainer);
    }
        break;
    case STATUS_PAUSE_PLAY: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileDisplayContainer);
        renameButton->hide();

        Utils::removeLayoutChild(actionLayout, 0);
        Utils::addLayoutWidget(actionLayout, pausePlayActionContainer);
    }
        break;
    }
}

QListWidgetItem* FileItem::getItem()
{
    return item;
}

int FileItem::getDuration() 
{
    int64_t duration = 0;
    
    av_register_all();
    AVFormatContext *pFormatCtx = avformat_alloc_context();
    avformat_open_input(&pFormatCtx, fileInfo.absoluteFilePath().toStdString().c_str(), NULL, NULL);
    
    if (pFormatCtx) {
        avformat_find_stream_info(pFormatCtx, NULL);
        duration = pFormatCtx->duration / 1000;
    }

    avformat_close_input(&pFormatCtx);
    avformat_free_context(pFormatCtx);
    
    return duration;
}

QString FileItem::getRecodingFilepath()
{
    return fileInfo.absoluteFilePath();
}
