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

#ifndef RECORDPAGE_H
#define RECORDPAGE_H

#include <QVBoxLayout>
#include "dimagebutton.h"

#include <QAudioRecorder>
#include <QAudioProbe>

#include "waveform.h"

#include "recording_button.h"

DWIDGET_USE_NAMESPACE

class RecordPage : public QWidget
{
    Q_OBJECT
    
public:
    RecordPage(QWidget *parent = 0);
    
    RecordingButton *recordingButton;
    QVBoxLayout *layout;
    DImageButton *finishButton;
    
    QString generateRecordingFilepath();
    QString getRecordingFilepath();
    
public slots:
    void renderLevel(const QAudioBuffer &buffer);
    void startRecord();
    void stopRecord();
    void pauseRecord();
    void resumeRecord();
    void renderRecordingTime();
    
private:
    QAudioRecorder *audioRecorder;
    QAudioProbe *audioProbe;
    
    Waveform *waveform;
    QLabel *recordTimeLabel;
    QLabel *titleLabel;
    
    float recordingTime;
    QDateTime lastUpdateTime;
    QTimer *tickerTimer;
    
    QString recordPath;
};

#endif
