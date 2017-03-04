#include <DTitlebar>
#include "main_window.h"
#include "toolbar.h"
#include "utils.h"

#include "home_page.h"
#include "record_page.h"
#include "list_page.h"

DWIDGET_USE_NAMESPACE

MainWindow::MainWindow(DMainWindow *parent) : DMainWindow(parent)
{
    this->titleBar()->setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowSystemMenuHint | Qt::WindowCloseButtonHint);
    
    Toolbar *toolbar = new Toolbar();
    this->titleBar()->setCustomWidget(toolbar, Qt::AlignVCenter, false);
    this->setFixedSize(440, 550);
    
    layoutWidget = new QWidget();
    this->setCentralWidget(layoutWidget);
    
    showHomePage();
}

void MainWindow::showHomePage()
{
    qDeleteAll(layoutWidget->children());
    
    homePage = new HomePage();
    connect(homePage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));
    
    layoutWidget->setLayout(homePage->layout);
}

void MainWindow::showRecordPage()
{
    qDeleteAll(layoutWidget->children());
    
    recordPage = new RecordPage();
    connect(recordPage->recordButton, SIGNAL(clicked()), this, SLOT(showListPage()));
    
    layoutWidget->setLayout(recordPage->layout);
}

void MainWindow::showListPage()
{
    qDeleteAll(layoutWidget->children());
    
    listPage = new ListPage();
    connect(listPage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));
    
    layoutWidget->setLayout(listPage->layout);
}
