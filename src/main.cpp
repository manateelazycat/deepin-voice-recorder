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

#include <DApplication>
#include <DMainWindow>
#include <QApplication>
#include <QDesktopWidget>
#include <QProcessEnvironment>
#include <QTranslator>
#include <dutility.h>

#include "main_window.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    DApplication app(argc, argv);

    if (app.setSingleInstance("deepin-voice-recorder")) {

#ifdef SNAP_APP
        // Load the qm files
        QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
        QString SNAP = env.value("SNAP");

        QTranslator* translator = new QTranslator();
        if (translator->load("deepin-voice-recorder_zh_CN.qm", SNAP + "/usr/share/deepin-voice-recorder/translations")) {
            app.installTranslator(translator);
        }
#else
        app.loadTranslator();
#endif

        app.setOrganizationName("deepin");
        app.setApplicationName(QObject::tr("Deepin Voice Recorder"));
        app.setApplicationVersion("1.0");
        
        app.setTheme("light");
        app.setWindowIcon(QIcon(Utils::getQrcPath("deepin-voice-recorder.svg")));

        MainWindow window;
        
        QObject::connect(&app, &DApplication::newInstanceStarted, &window, &MainWindow::activateWindow);
        
        Utils::applyQss(&window, "main.qss");
        DUtility::moveToCenter(&window);
        window.show();

        return app.exec();
    }

    return 0;
}
