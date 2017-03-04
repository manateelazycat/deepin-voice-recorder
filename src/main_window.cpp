#include <DTitlebar>
#include "main_window.h"
#include "toolbar.h"

#include "home_page.h"
#include "record_page.h"

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(DMainWindow *parent) : DMainWindow(parent)
{
    this->titleBar()->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    
    Toolbar *toolbar = new Toolbar();
    this->titleBar()->setCustomWidget(toolbar, Qt::AlignVCenter, false);
    this->setFixedSize(440, 550);
    
    homePage = new HomePage();
    this->setCentralWidget(homePage);
    
    connect(homePage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));
}


void MainWindow::showRecordPage()
{
    recordPage = new RecordPage();
    this->setCentralWidget(recordPage);
}
