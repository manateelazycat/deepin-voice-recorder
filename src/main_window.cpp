#include <DTitlebar>
#include "main_window.h"
#include "toolbar.h"
#include "utils.h"

#include <QDebug>

#include "home_page.h"
#include "record_page.h"
#include "list_page.h"
#include "edit_page.h"

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
    Utils::removeChildren(layoutWidget);
    
    homePage = new HomePage();
    connect(homePage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));
    
    layoutWidget->setLayout(homePage->layout);
}

void MainWindow::showRecordPage()
{
    Utils::removeChildren(layoutWidget);
    
    recordPage = new RecordPage();
    
    layoutWidget->setLayout(recordPage->layout);
}

void MainWindow::showListPage()
{
    Utils::removeChildren(layoutWidget);
    
    listPage = new ListPage();
    connect(listPage->recordButton, SIGNAL(clicked()), this, SLOT(showRecordPage()));
    connect(listPage->tempButton, SIGNAL(clicked()), this, SLOT(showEditPage()));
    
    layoutWidget->setLayout(listPage->layout);
}

void MainWindow::showEditPage()
{
    Utils::removeChildren(layoutWidget);
    
    editPage = new EditPage();
    connect(editPage->editButton, SIGNAL(clicked()), this, SLOT(showListPage()));
    
    layoutWidget->setLayout(editPage->layout);
}
