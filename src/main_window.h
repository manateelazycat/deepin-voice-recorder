/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2018 Deepin, Inc.
 *               2011 ~ 2018 Wang Yong
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QStackedLayout>

#include "dmainwindow.h"
#include "home_page.h"
#include "list_page.h"
#include "record_page.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT
    
    static const int PAGE_TYPE_HOME;
    static const int PAGE_TYPE_RECORD;
    static const int PAGE_TYPE_LIST;
    
public:
    MainWindow(DMainWindow *parent = 0);
    
public slots:
    void newRecord();
    void openSaveDirectory();
    void showFirstPage();
    void showHomePage();
    void showListPage(QString recordingPath);
    void showRecordPage();
    
private:
    HomePage *homePage;
    ListPage *listPage;
    QAction *newRecordAction;
    QAction *openSaveDirectoryAction;
    QMenu *menu;
    QStackedLayout *stackedLayout;
    QVBoxLayout *layout;
    QWidget *layoutWidget;
    RecordPage *recordPage;
    int pageType;
};

#endif
