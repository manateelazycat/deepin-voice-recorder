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
#include <DWidgetUtil>
#include <DHiDPIHelper>

#include "main_window.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    const char *descriptionText = QT_TRANSLATE_NOOP("MainWindow", "Deepin Voice Recorder is a beautiful and "
                                                    "easy to use voice recording application "
                                                    "with simple design. It supports visual "
                                                    "recording, recording playback, recording "
                                                    "list management and other functions.");

    const QString acknowledgementLink = "https://www.deepin.org/acknowledgments/deepin-voice-recorder#thanks";

    DApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps);

    if (app.setSingleInstance("deepin-voice-recorder")) {
        app.loadTranslator();

        app.setOrganizationName("deepin");
        app.setApplicationName("deepin-voice-recorder");
        app.setApplicationVersion("1.0");

        app.setProductIcon(QIcon(Utils::getQrcPath("logo_96.svg")));
        app.setProductName(DApplication::translate("MainWindow", "Deepin Voice Recorder"));
        app.setApplicationDescription(DApplication::translate("MainWindow", descriptionText) + "\n");
        app.setApplicationAcknowledgementPage(acknowledgementLink);

        app.setTheme("light");
        app.setWindowIcon(QIcon(Utils::getQrcPath("deepin-voice-recorder.svg")));

        MainWindow window;

        QObject::connect(&app, &DApplication::newInstanceStarted, &window, &MainWindow::activateWindow);

        Utils::applyQss(&window, "main.qss");
        Dtk::Widget::moveToCenter(&window);
        window.show();

        return app.exec();
    }

    return 0;
}
