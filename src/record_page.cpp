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

#include <QAudioEncoderSettings>
#include <QAudioProbe>
#include <QAudioRecorder>
#include <QDate>
#include <QDebug>
#include <QWidget>
#include <QDir>
#include <QFont>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QStandardPaths>
#include <QTime>
#include <QUrl>
#include <QVBoxLayout>
#include <QApplication>
#include <DHiDPIHelper>

#include "dimagebutton.h"
#include "record_page.h"
#include "recording_button.h"
#include "utils.h"
#include "waveform.h"

DWIDGET_USE_NAMESPACE

RecordPage::RecordPage(QWidget *parent) : QWidget(parent)
{
    installEventFilter(this);  // add event filter

    recordingTime = 0;

    layout = new QVBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    titleLabel = new QLabel(tr("New recording"));
    QFont titleFont;
    titleFont.setPixelSize(26);
    titleLabel->setFont(titleFont);
    waveform = new Waveform();
    QFont recordTimeFont;
    recordTimeFont.setPixelSize(14);
    recordTimeLabel = new QLabel("00:00");
    recordTimeLabel->setFont(recordTimeFont);

    buttonAreaWidget = new QWidget();
    buttonAreaLayout = new QVBoxLayout();
    buttonAreaLayout->setContentsMargins(0, 0, 0, 0);

    buttonWidget = new QWidget();
    buttonLayout = new QHBoxLayout(buttonWidget);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    expandAnimationButtonLayout = new QVBoxLayout();
    expandAnimationButtonLayout->setContentsMargins(0, 0, 0, 0);

    expandAnimationButton = new ExpandAnimationButton();
    connect(expandAnimationButton, &ExpandAnimationButton::finish, this, &RecordPage::handleExpandAnimationFinish);

    expandAnimationButtonLayout->addWidget(expandAnimationButton, 0, Qt::AlignHCenter);

    shrankAnimationButtonLayout = new QVBoxLayout();
    shrankAnimationButtonLayout->setContentsMargins(0, 0, 0, 0);

    shrankAnimationButton = new ShrankAnimationButton();
    connect(shrankAnimationButton, &ShrankAnimationButton::finish, this, &RecordPage::handleShrankAnimationFinish);

    shrankAnimationButtonLayout->addWidget(shrankAnimationButton, 0, Qt::AlignHCenter);

    recordingButton = new RecordingButton();

    finishButton = new DImageButton(
        Utils::getQrcPath("finish_normal.svg"),
        Utils::getQrcPath("finish_hover.svg"),
        Utils::getQrcPath("finish_press.svg")
    );

    buttonLayout->addStretch();
    buttonLayout->addWidget(recordingButton, 0, Qt::AlignVCenter);
    buttonLayout->addSpacing(20);
    buttonLayout->addWidget(finishButton, 0, Qt::AlignVCenter);
    buttonLayout->addStretch();

    buttonAreaLayout->addWidget(buttonWidget, 0, Qt::AlignHCenter);

    layout->addSpacing(36);
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(waveform, 1, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(recordTimeLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(buttonAreaWidget);
    layout->addSpacing(30);     // NOTE: bottom buttons padding

    audioRecorder = new QAudioRecorder(this);
    qDebug() << "support codecs:" << audioRecorder->supportedAudioCodecs();
    qDebug() << "support containers:" << audioRecorder->supportedContainers();

    QAudioEncoderSettings audioSettings;
    audioSettings.setQuality(QMultimedia::HighQuality);

#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
    audioRecorder->setAudioSettings(audioSettings);
    audioRecorder->setContainerFormat("audio/x-wav");
#else
    audioSettings.setCodec("audio/PCM");
    audioRecorder->setAudioSettings(audioSettings);
    audioRecorder->setContainerFormat("wav");
#endif

    audioProbe = new QAudioProbe(this);
    if (audioProbe->setSource(audioRecorder)) {
        connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(renderLevel(QAudioBuffer)));
    }

    tickerTimer = new QTimer();
    connect(tickerTimer, SIGNAL(timeout()), this, SLOT(renderRecordingTime()));
    tickerTimer->start(1000);

    startRecord();

    connect(finishButton, SIGNAL(clicked()), this, SLOT(handleClickFinishButton()));
    connect(recordingButton, SIGNAL(pause()), this, SLOT(pauseRecord()));
    connect(recordingButton, SIGNAL(resume()), this, SLOT(resumeRecord()));

    QFileInfoList fileInfoList = Utils::getRecordingFileinfos();
    if (fileInfoList.count() == 0) {
        buttonAreaWidget->setLayout(buttonAreaLayout);

        // Get keyboard focus.
        setFocus();
    } else {
        buttonAreaWidget->setLayout(expandAnimationButtonLayout);
        expandAnimationButton->startAnimation();
    }
}

void RecordPage::handleExpandAnimationFinish()
{
    Utils::removeChildren(buttonAreaWidget);

    buttonAreaWidget->setLayout(buttonAreaLayout);

    // Get keyboard focus.
    setFocus();
}

void RecordPage::handleShrankAnimationFinish()
{
    emit finishRecord(getRecordingFilepath());
}

void RecordPage::handleClickFinishButton()
{
    stopRecord();

    Utils::removeChildren(buttonAreaWidget);

    buttonAreaWidget->setLayout(shrankAnimationButtonLayout);
    shrankAnimationButton->startAnimation();
}

void RecordPage::renderRecordingTime()
{
    if (audioRecorder->state() != QMediaRecorder::StoppedState) {
        recordTimeLabel->setText(Utils::formatMillisecond(recordingTime));
    }
}

void RecordPage::startRecord()
{
    recordPath = generateRecordingFilepath();
    audioRecorder->setOutputLocation(recordPath);

    QDateTime currentTime = QDateTime::currentDateTime();
    lastUpdateTime = currentTime;
    audioRecorder->record();
}

void RecordPage::stopRecord()
{
    audioRecorder->stop();
    tickerTimer->stop();
}

void RecordPage::exitRecord()
{
    stopRecord();

    QFile(getRecordingFilepath()).remove();

    emit cancelRecord();
}

void RecordPage::pauseRecord()
{
    audioRecorder->pause();
}

void RecordPage::resumeRecord()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    lastUpdateTime = currentTime;

    audioRecorder->record();
}

QString RecordPage::generateRecordingFilepath()
{
    return QDir(Utils::getRecordingSaveDirectory()).filePath(QString("%1 (%2).wav").arg(tr("New recording")).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")));
}

QString RecordPage::getRecordingFilepath()
{
    return recordPath;
}

void RecordPage::renderLevel(const QAudioBuffer &buffer)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    recordingTime += lastUpdateTime.msecsTo(currentTime);
    lastUpdateTime = currentTime;

    QVector<qreal> levels = Waveform::getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
        waveform->updateWave(levels.at(i));
    }
}

bool RecordPage::eventFilter(QObject *, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent == QKeySequence::Cancel) {
            exitRecord();
        }
    }

    return false;
}
