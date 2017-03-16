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

const int MainWindow::PAGE_TYPE_HOME = 1;
const int MainWindow::PAGE_TYPE_RECORD = 2;
const int MainWindow::PAGE_TYPE_LIST = 3;

MainWindow::MainWindow(DMainWindow *parent) : DMainWindow(parent)
{
    // Make window can close by alt+f4.
    setWindowFlags(Qt::FramelessWindowHint  | Qt::WindowCloseButtonHint);
    
    menu = new QMenu();
    newRecordAction = new QAction(tr("New recording"), this);
    connect(newRecordAction, &QAction::triggered, this, &MainWindow::newRecord);
    openSaveDirectoryAction = new QAction(tr("Open saved directory"), this);
    connect(openSaveDirectoryAction, &QAction::triggered, this, &MainWindow::openSaveDirectory);
    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);
    helpAction = new QAction(tr("Help"), this);
    connect(helpAction, &QAction::triggered, this, &MainWindow::showHelpManual);
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, &QAction::triggered, this, &MainWindow::exit);
    menu->addAction(newRecordAction);
    menu->addSeparator();
    menu->addAction(openSaveDirectoryAction);
    menu->addAction(aboutAction);
    menu->addAction(helpAction);
    menu->addAction(quitAction);

    this->titleBar()->setMenu(menu);
    this->titleBar()->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);

    Toolbar *toolbar = new Toolbar();
    this->titleBar()->setCustomWidget(toolbar, Qt::AlignVCenter, false);
    this->setFixedSize(440, 550);

    layoutWidget = new QWidget();
    this->setCentralWidget(layoutWidget);
    
    stackedLayout = new QStackedLayout();
    layoutWidget->setLayout(stackedLayout);

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
        delete currentWidget;
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
        delete currentWidget;
    }
    
    recordPage = new RecordPage();
    connect(recordPage, &RecordPage::finishRecord, this, &MainWindow::showListPage);

    stackedLayout->addWidget(recordPage);
}

void MainWindow::showListPage(QString recordingPath)
{
    pageType = PAGE_TYPE_LIST;
    
    QWidget *currentWidget = stackedLayout->currentWidget();
    if (currentWidget != 0) {
        delete currentWidget;
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
    }
    
    showRecordPage();
}

void MainWindow::openSaveDirectory()
{
    QFileInfo ddefilemanger("/usr/bin/dde-file-manager");
    if (ddefilemanger.exists()) {
        QProcess::startDetached("dde-file-manager", QStringList() << Utils::getRecordingSaveDirectory());
    } else {
        QProcess::startDetached("gvfs-open", QStringList() << Utils::getRecordingSaveDirectory());
    }
}

void MainWindow::showAbout()
{
    QString descriptionText = tr("Deepin Voice Recorder is a beautiful and"
                                 "easy to use voice recording application "
                                 "with simple design. It supports visual "
                                 "recording, recording playback, recording "
                                 "list management and other functions."
                                 );
    QString acknowledgementLink = "https://www.deepin.org/acknowledgments/deepin-voice-recorder#thanks";

    auto *aboutDlg = new Dtk::Widget::DAboutDialog();
    aboutDlg->setWindowModality(Qt::WindowModal);
    aboutDlg->setWindowIcon(QPixmap::fromImage(QImage(Utils::getQrcPath("logo.png"))));
    aboutDlg->setProductIcon(QPixmap::fromImage(QImage(Utils::getQrcPath("logo_96.png"))));
    aboutDlg->setProductName(tr("Deepin Voice Recorder"));
    aboutDlg->setVersion(QString("%1: 1.0").arg(tr("Version")));
    aboutDlg->setDescription(descriptionText + "\n");
    aboutDlg->setAcknowledgementLink(acknowledgementLink);
    aboutDlg->show();
}

void MainWindow::showHelpManual()
{
    static QProcess *m_manual = nullptr;
    if (NULL == m_manual) {
        m_manual =  new QProcess();
        const QString pro = "dman";
        const QStringList args("deepin-voice-recorder");
        m_manual->connect(m_manual, static_cast<void(QProcess::*)(int)>(&QProcess::finished), this, [ = ](int) {
                m_manual->deleteLater();
                m_manual = nullptr;
            });
        m_manual->start(pro, args);
    }
}

void MainWindow::exit()
{
    QApplication::quit();
}
