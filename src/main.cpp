#include <QApplication>
#include <QDesktopWidget>
#include <DApplication>
#include <DMainWindow>
#include <dutility.h>

#include "main_window.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[]) 
{
    DApplication::loadDXcbPlugin();
    
    DApplication app(argc, argv);
    app.setTheme("light");
    
    MainWindow window;
    DUtility::moveToCenter(&window);
    window.show();
    
    return app.exec();
}
