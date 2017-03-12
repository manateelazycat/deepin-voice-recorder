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

#ifndef FILEITEM_H
#define FILEITEM_H

#include <QFileInfo>
#include <QWidget>
#include <QListWidgetItem>
#include "dimagebutton.h"
#include <QHBoxLayout>
#include <QLabel>
#include "line_edit.h"

DWIDGET_USE_NAMESPACE

class FileItem : public QWidget
{
    Q_OBJECT
    
public:
    FileItem(QWidget *parent=0);
    void setFileInfo(QFileInfo info);
    QFileInfo getFileInfo();
    int getDuration();
    QListWidgetItem* getItem();
    QString getRecodingFilepath();
    
    static const int STATUS_NORMAL;
    static const int STATUS_RENAME;
    static const int STATUS_PLAY;
    static const int STATUS_PLAY_PAUSE;
    static const int STATUS_PAUSE_PLAY;
    
public slots:
    void switchStatus(int status);
    
signals:
    void play();
    void pause();
    void resume();
    void stop();
    
private:
    QHBoxLayout *layout;
    QHBoxLayout *infoLayout;
    QHBoxLayout *actionLayout;
    
    QLabel *fileIcon;
    QLabel *fileName;
    QLabel *durationLabel;
    
    LineEdit *lineEdit;
    DImageButton *renameButton;
    DImageButton *playStartButton;
    
    DImageButton *playPauseButton;
    DImageButton *playStopButton;

    DImageButton *pausePlayButton;
    DImageButton *pauseStopButton;

    QWidget *fileDisplayContainer;
    QHBoxLayout *fileDisplayLayout;
    QWidget *fileRenameContainer;
    QHBoxLayout *fileRenameLayout;
    
    QWidget *normalActionContainer;
    QHBoxLayout *normalActionLayout;
    QWidget *playActionContainer;
    QHBoxLayout *playActionLayout;
    QWidget *pausePlayActionContainer;
    QHBoxLayout *pausePlayActionLayout;
    QWidget *playPauseActionContainer;
    QHBoxLayout *playPauseActionLayout;
    
    QFileInfo fileInfo;
    
    QListWidgetItem *item;
};

#endif
