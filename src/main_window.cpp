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

#include <DDesktopServices>
#include <DTitlebar>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QStandardPaths>
#include <daboutdialog.h>

#include "home_page.h"
#include "list_page.h"
#include "main_window.h"
#include "record_page.h"
#include "toolbar.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE
DCORE_USE_NAMESPACE

const int MainWindow::PAGE_TYPE_HOME = 1;
const int MainWindow::PAGE_TYPE_RECORD = 2;
const int MainWindow::PAGE_TYPE_LIST = 3;

MainWindow::MainWindow(DMainWindow *parent) : DMainWindow(parent)
{
    menu = new QMenu();
    newRecordAction = new QAction(tr("New recording"), this);
    connect(newRecordAction, &QAction::triggered, this, &MainWindow::newRecord);
    openSaveDirectoryAction = new QAction(tr("Open saved directory"), this);
    connect(openSaveDirectoryAction, &QAction::triggered, this, &MainWindow::openSaveDirectory);

    menu->addAction(newRecordAction);
    menu->addSeparator();
    menu->addAction(openSaveDirectoryAction);

    if (this->titlebar()) {
        this->titlebar()->setMenu(menu);

        Toolbar *toolbar = new Toolbar();
        this->titlebar()->setCustomWidget(toolbar, Qt::AlignVCenter, false);
        this->setFixedSize(440, 550);
    }
    
    Utils::applyQss(this->titlebar(), "main.qss");

    layoutWidget = new QWidget();
    this->setCentralWidget(layoutWidget);

    stackedLayout = new QStackedLayout();
    layoutWidget->setLayout(stackedLayout);

    showFirstPage();
}

void MainWindow::showFirstPage()
{
    QFileInfoList fileInfoList = Utils::getRecordingFileinfos();

    if (fileInfoList.size() > 0) {
        showListPage("");
    } else {
        showHomePage();
    }
}

void MainWindow::showHomePage()
{
    pageType = PAGE_TYPE_HOME;

    QWidget *currentWidget = stackedLayout->currentWidget();
    if (currentWidget != 0) {
        currentWidget->deleteLater();
    }

    homePage = new HomePage();
    connect(homePage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));

    stackedLayout->addWidget(homePage);
}

void MainWindow::showRecordPage()
{
    pageType = PAGE_TYPE_RECORD;

    QWidget *currentWidget = stackedLayout->currentWidget();
    if (currentWidget != 0) {
        currentWidget->deleteLater();
    }

    recordPage = new RecordPage();
    connect(recordPage, &RecordPage::finishRecord, this, &MainWindow::showListPage);
    connect(recordPage, &RecordPage::cancelRecord, this, &MainWindow::showFirstPage);

    stackedLayout->addWidget(recordPage);
}

void MainWindow::showListPage(QString recordingPath)
{
    pageType = PAGE_TYPE_LIST;

    QWidget *currentWidget = stackedLayout->currentWidget();
    if (currentWidget != 0) {
        currentWidget->deleteLater();
    }

    listPage = new ListPage();
    connect(listPage, SIGNAL(clickRecordButton()), this, SLOT(showRecordPage()));
    connect(listPage->fileView, &FileView::listClear, this, &MainWindow::showHomePage);

    if (recordingPath != "") {
        listPage->selectItemWithPath(recordingPath);
    }

    stackedLayout->addWidget(listPage);
}

void MainWindow::newRecord()
{
    if (pageType == PAGE_TYPE_RECORD) {
        recordPage->stopRecord();
    } else if (pageType == PAGE_TYPE_LIST) {
        listPage->stopPlayer();
    }

    showRecordPage();
}

void MainWindow::openSaveDirectory()
{
    DDesktopServices::showFolder(Utils::getRecordingSaveDirectory());
}

