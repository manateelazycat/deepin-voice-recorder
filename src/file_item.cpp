/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef __cplusplus
extern "C" {
#endif
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#ifdef __cplusplus
}
#endif

#include <QDebug>
#include <QDir>
#include <QEvent>
#include <QTimer>
#include <QWidget>
#include <QThread>

#include "file_item.h"
#include "label.h"
#include "utils.h"

const int FileItem::STATUS_NORMAL = 0;
const int FileItem::STATUS_RENAME = 1;
const int FileItem::STATUS_PLAY = 2;
const int FileItem::STATUS_PLAY_PAUSE = 3;
const int FileItem::STATUS_PAUSE_PLAY = 4;

FileItem::FileItem(QWidget *parent) : QWidget(parent)
{
    installEventFilter(this);  // add event filter
    setMouseTracking(true);   // make MouseMove can response

    currentStatus = STATUS_NORMAL;
    switchLock = false;

    isEntered = false;

    nameTemplate = "<span style='font-size:14px; font-weight:400; color:#000000;'>%1</span>";
    durationTemplate = "<span style='font-size:11px; color:#808080'>%1</span>";

    item = new QListWidgetItem();
    layout = new QHBoxLayout();
    infoLayout = new QHBoxLayout();
    actionLayout = new QHBoxLayout();

    fileIcon = new Label();
    connect(fileIcon, &Label::mouseMove, this, &FileItem::switchPlay);
    fileIcon->setPixmap(QPixmap::fromImage(QImage(Utils::getQrcPath("file.png"))));

    nameLabel = new Label();
    connect(nameLabel, &Label::mouseMove, this, &FileItem::switchPlay);
    lineEdit = new LineEdit();

    durationLabel = new Label();
    connect(durationLabel, &Label::mouseMove, this, &FileItem::switchPlay);
    
    playStartButton = new DImageButton(
        Utils::getQrcPath("play_start_normal.png"),
        Utils::getQrcPath("play_start_hover.png"),
        Utils::getQrcPath("play_start_press.png")
        );

    playPauseButton = new DImageButton(
        Utils::getQrcPath("play_pause_normal.png"),
        Utils::getQrcPath("play_pause_hover.png"),
        Utils::getQrcPath("play_pause_press.png")
        );

    playStopButton = new DImageButton(
        Utils::getQrcPath("play_stop_normal.png"),
        Utils::getQrcPath("play_stop_hover.png"),
        Utils::getQrcPath("play_stop_press.png")
        );

    pausePlayButton = new DImageButton(
        Utils::getQrcPath("play_start_normal.png"),
        Utils::getQrcPath("play_start_hover.png"),
        Utils::getQrcPath("play_start_press.png")
        );

    pauseStopButton = new DImageButton(
        Utils::getQrcPath("play_stop_normal.png"),
        Utils::getQrcPath("play_stop_hover.png"),
        Utils::getQrcPath("play_stop_press.png")
        );

    setLayout(layout);
    layout->addLayout(infoLayout);
    layout->addLayout(actionLayout);
    layout->setContentsMargins(13, 0, 5, 0);

    infoLayout->addWidget(fileIcon);

    fileDisplayContainer = new QWidget();
    fileDisplayLayout = new QHBoxLayout(fileDisplayContainer);
    fileDisplayLayout->addWidget(nameLabel);
    fileDisplayLayout->addStretch();

    fileRenameContainer = new QWidget();
    fileRenameLayout = new QHBoxLayout(fileRenameContainer);
    fileRenameLayout->addWidget(lineEdit);

    normalActionContainer = new QWidget();
    normalActionLayout = new QHBoxLayout(normalActionContainer);
    normalActionLayout->addWidget(durationLabel);

    playActionContainer = new QWidget();
    playActionLayout = new QHBoxLayout(playActionContainer);
    playActionLayout->addWidget(playStartButton);

    playPauseActionContainer = new QWidget();
    playPauseActionLayout = new QHBoxLayout(playPauseActionContainer);
    playPauseActionLayout->addWidget(playStopButton);
    playPauseActionLayout->addWidget(playPauseButton);

    pausePlayActionContainer = new QWidget();
    pausePlayActionLayout = new QHBoxLayout(pausePlayActionContainer);
    pausePlayActionLayout->addWidget(pauseStopButton);
    pausePlayActionLayout->addWidget(pausePlayButton);

    switchStatus(STATUS_NORMAL);

    connect(lineEdit, &QLineEdit::editingFinished, [=] () {
            QString newFilename = lineEdit->text();
            
            if (newFilename != fileInfo.baseName()) {
                QString newFilepath = fileInfo.absoluteDir().filePath(QString("%1.wav").arg(newFilename));
                
                if (!Utils::fileExists(newFilepath) && newFilename.trimmed() != "" && !newFilename.contains('/')) {
                    QString oldFilepath = fileInfo.absoluteFilePath();

                    fileInfo = QFileInfo(newFilepath);
                    QFile(oldFilepath).rename(newFilepath);
                    nameLabel->setText(QString(nameTemplate).arg(newFilename));
                }
            }
            
            switchStatus(STATUS_PLAY);
        });
    connect(lineEdit, &LineEdit::pressEsc, [=] () {
            // Redo edit operation.
            lineEdit->setText(fileInfo.baseName());
            
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

void FileItem::leaveEvent(QEvent *event)
{
    if (currentStatus == STATUS_PLAY) {
        switchStatus(STATUS_NORMAL);
    }

    isEntered = false;
    repaint();
    
    QWidget::leaveEvent(event);
}

void FileItem::enterEvent(QEvent *event)
{
    switchPlay();

    isEntered = true;
    repaint();
    
    QWidget::enterEvent(event);
}

bool FileItem::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::MouseMove) {
        switchPlay();
    }
    
    return false;
}

void FileItem::switchPlay()
{
    // Don't call 'switchStatus' once switchLock is lock.
    // Avoid call 'switchStatus' recursively.
    if (!switchLock) {
        if (currentStatus == STATUS_NORMAL) {
            switchStatus(STATUS_PLAY);
        }
    }
}

void FileItem::paintEvent(QPaintEvent *event)
{
    if (isEntered) {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setOpacity(0.05);

        QPainterPath path;
        path.addRoundedRect(QRectF(rect()), 5, 5);
        painter.fillPath(path, Qt::black);
    }

    QWidget::paintEvent(event);
}

void FileItem::setFileInfo(QFileInfo info)
{
    fileInfo = info;
    nameLabel->setText(QString(nameTemplate).arg(info.baseName()));

    updateDurationLabel();
}

void FileItem::updateDurationLabel()
{
    durationLabel->setText(QString(durationTemplate).arg(Utils::formatMillisecond(getDuration())));
}

QFileInfo FileItem::getFileInfo()
{
    return fileInfo;
}

void FileItem::switchStatus(int status)
{
    switchLock = true;
    
    currentStatus = status;

    switch(status) {
    case STATUS_NORMAL: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileDisplayContainer);

        Utils::removeLayoutChild(actionLayout, 0);
        Utils::addLayoutWidget(actionLayout, normalActionContainer);
    }
        break;
    case STATUS_RENAME: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileRenameContainer);
        QTimer::singleShot(0, lineEdit, SLOT(setFocus()));

        lineEdit->setText(fileInfo.baseName());
        lineEdit->selectAll();
    }
        break;
    case STATUS_PLAY: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileDisplayContainer);

        Utils::removeLayoutChild(actionLayout, 0);
        Utils::addLayoutWidget(actionLayout, playActionContainer);;
    }
        break;
    case STATUS_PLAY_PAUSE: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileDisplayContainer);

        Utils::removeLayoutChild(actionLayout, 0);
        Utils::addLayoutWidget(actionLayout, playPauseActionContainer);
    }
        break;
    case STATUS_PAUSE_PLAY: {
        Utils::removeLayoutChild(infoLayout, 1);
        Utils::addLayoutWidget(infoLayout, fileDisplayContainer);

        Utils::removeLayoutChild(actionLayout, 0);
        Utils::addLayoutWidget(actionLayout, pausePlayActionContainer);
    }
        break;
    }
    
    switchLock = false;
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
