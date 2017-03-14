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

#include <QDebug>
#include <QVBoxLayout>

#include "dimagebutton.h"
#include "recording_button.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

RecordingButton::RecordingButton(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout();
    setLayout(layout);
    
    pauseButton = new DImageButton(
        Utils::getQrcPath("record_pause_normal.png"),
        Utils::getQrcPath("record_pause_hover.png"),
        Utils::getQrcPath("record_pause_press.png")
        );
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(handlePause()));

    resumeButton = new DImageButton(
        Utils::getQrcPath("record_resume_normal.png"),
        Utils::getQrcPath("record_resume_hover.png"),
        Utils::getQrcPath("record_resume_press.png")
        );
    connect(resumeButton, SIGNAL(clicked()), this, SLOT(handleResume()));
    
    layout->addWidget(pauseButton, 0, Qt::AlignCenter);
}

void RecordingButton::handlePause() {
    layout->removeWidget(pauseButton);
    pauseButton->setParent(NULL);
    layout->addWidget(resumeButton, 0, Qt::AlignCenter);
    emit pause();
}

void RecordingButton::handleResume() {
    layout->removeWidget(resumeButton);
    resumeButton->setParent(NULL);
    layout->addWidget(pauseButton, 0, Qt::AlignCenter);
    emit resume();
}
