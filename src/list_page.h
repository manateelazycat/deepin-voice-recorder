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

#ifndef LISTPAGE_H
#define LISTPAGE_H

#include <QAudioProbe>
#include <QMediaPlayer>
#include <QVBoxLayout>

#include "dimagebutton.h"
#include "file_view.h"
#include "waveform.h"

DWIDGET_USE_NAMESPACE

class ListPage : public QWidget
{
    Q_OBJECT
    
public:
    ListPage(QWidget *parent = 0);
    
    DImageButton *recordButton;
    FileView *fileView;
    QVBoxLayout *layout;
                       
    QString getPlayingFilepath();
    
public slots:
    void handleClickRecordButton();
    void handleStateChanged(QMediaPlayer::State state);
    void pause(QString filepath);
    void play(QString filepath);
    void renderLevel(const QAudioBuffer &buffer);
    void resume(QString filepath);
    void selectItemWithPath(QString path);
    void stop(QString filepath);
    void stopPlayer();
    
signals:
    void clickRecordButton();
    void playFinished(QString filepath);
    
private:    
    QAudioProbe *audioProbe;
    QMediaPlayer *audioPlayer;
    Waveform *waveform;
};

#endif
