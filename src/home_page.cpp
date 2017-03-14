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

#include <QVBoxLayout>

#include "dimagebutton.h"
#include "home_page.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    recordButton = new DImageButton(
        Utils::getQrcPath("home_page_record_normal.png"),
        Utils::getQrcPath("home_page_record_hover.png"),
        Utils::getQrcPath("home_page_record_press.png")
        );

    layout = new QVBoxLayout();
    setLayout(layout);

    layout->addWidget(recordButton, 0, Qt::AlignCenter);
}
