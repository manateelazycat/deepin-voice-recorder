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

#include <QPainter>
#include <QGraphicsOpacityEffect>

#include <QDebug>

#include "animation_button.h"
#include "utils.h"

AnimationButton::AnimationButton(QWidget *parent) : QWidget(parent)
{
    pauseButtonImg = QImage(Utils::getQrcPath("record_pause_normal.png"));
    finishButtonImg = QImage(Utils::getQrcPath("finish_normal.png"));

    setFixedSize(300, pauseButtonImg.height());

    renderTicker = 0;
    opacityFrames = 5;
    animationFrames = 8;
    animationDuration = 25;
    
    renderTimer = new QTimer();
    connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderAnimation()));
    renderTimer->start(animationDuration);
}

void AnimationButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    if (renderTicker <= animationFrames) {
        painter.setOpacity(1 * Utils::easeOutQuad(std::min(renderTicker, opacityFrames) / (opacityFrames * 1.0)));
        painter.drawImage(QPoint((rect().width() - pauseButtonImg.width()) / 2 - Utils::easeOutQuad((renderTicker) / (animationFrames * 1.0)) * 40, 
                                 (rect().height() - pauseButtonImg.height()) / 2),
                          pauseButtonImg);

        painter.setOpacity(1 * Utils::easeOutQuad(std::min(renderTicker, opacityFrames) / (opacityFrames * 1.0)));
        painter.drawImage(QPoint((rect().width() - finishButtonImg.width()) / 2 + Utils::easeOutQuad((renderTicker) / (animationFrames * 1.0)) * 40, 
                                 (rect().height() - finishButtonImg.height()) / 2),
                          finishButtonImg);
    }
}

void AnimationButton::renderAnimation()
{
    if (renderTicker < animationFrames) {
        renderTicker++;

        repaint();
    } else {
        renderTimer->stop();
        
        emit finish();
    }
}

