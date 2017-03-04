#include <QApplication>
#include <QDesktopWidget>
#include <DApplication>
#include <DTitlebar>
#include <DMainWindow>
#include <dutility.h>

#include "toolbar.h"

DWIDGET_USE_NAMESPACE

int main(int argc, char *argv[]) 
{
    DApplication::loadDXcbPlugin();
    
    DApplication app(argc, argv);
    app.setTheme("light");
    
    DMainWindow window;
    window.titleBar()->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    
    Toolbar *toolbar = new Toolbar();
    window.titleBar()->setCustomWidget(toolbar, Qt::AlignVCenter, false);
    window.setFixedSize(440, 550);
    
    DUtility::moveToCenter(&window);
    window.show();
    
    return app.exec();
}
