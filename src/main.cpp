#include <QApplication>
#include <QDesktopWidget>
#include <DApplication>
#include <DMainWindow>
#include <dutility.h>

#include "main_window.h"
#include "utils.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();

    DApplication app(argc, argv);

    if (app.setSingleInstance("deepin-voice-recorder")) {
        app.setOrganizationName("deepin");
        app.setApplicationName("deepin-voice-recorder");
        app.setApplicationVersion("1.0");
        
        app.loadTranslator();
        
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
