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
#include <QPainter>
#include <DHiDPIHelper>
#include <QApplication>

#include "expand_animation_button.h"
#include "utils.h"

const int ExpandAnimationButton::ANIMATION_WIDTH = 45;

DWIDGET_USE_NAMESPACE

ExpandAnimationButton::ExpandAnimationButton(QWidget *parent) : QWidget(parent)
{
    pauseButtonImg = DHiDPIHelper::loadNxPixmap(Utils::getQrcPath("record_pause_normal.svg"));
    finishButtonImg = DHiDPIHelper::loadNxPixmap(Utils::getQrcPath("finish_normal.svg"));

    setFixedSize(300, 64);

    renderTicker = 0;
    opacityFrames = 5;
    animationFrames = 8;
    animationDuration = 25;
}

void ExpandAnimationButton::startAnimation()
{
    renderTimer = new QTimer();
    connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderAnimation()));
    renderTimer->start(animationDuration);
}

void ExpandAnimationButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    
    qreal devicePixelRatio = qApp->devicePixelRatio();
    if (renderTicker <= animationFrames) {
        painter.setOpacity(1 * Utils::easeOutQuad(std::min(renderTicker, opacityFrames) / (opacityFrames * 1.0)));
        painter.drawPixmap(QPoint((rect().width() - pauseButtonImg.width() / devicePixelRatio) / 2 - Utils::easeOutQuad((renderTicker) / (animationFrames * 1.0)) * ANIMATION_WIDTH,
                                  (rect().height() - pauseButtonImg.height() / devicePixelRatio) / 2),
                           pauseButtonImg);

        painter.setOpacity(1 * Utils::easeOutQuad(std::min(renderTicker, opacityFrames) / (opacityFrames * 1.0)));
        painter.drawPixmap(QPoint((rect().width() - finishButtonImg.width() / devicePixelRatio) / 2 + Utils::easeOutQuad((renderTicker) / (animationFrames * 1.0)) * ANIMATION_WIDTH,
                                  (rect().height() - finishButtonImg.height() / devicePixelRatio) / 2),
                           finishButtonImg);
    }
}

void ExpandAnimationButton::renderAnimation()
{
    if (renderTicker < animationFrames) {
        renderTicker++;

        repaint();
    } else {
        renderTimer->stop();

        emit finish();
    }
}
