#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dmainwindow.h"

#include "home_page.h"
#include "record_page.h"
#include "list_page.h"
#include "edit_page.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(DMainWindow *parent = 0);
    
public slots:
    void showHomePage();
    void showRecordPage();
    void showListPage();
    void showEditPage();
    
    void newRecord();
    void openSaveDirectory();
    void showAbout();
    void showHelpManual();
    void exit();
    
private:
    QWidget *layoutWidget;
    QVBoxLayout *layout;
    
    HomePage *homePage;
    RecordPage *recordPage;
    ListPage *listPage;
    EditPage *editPage;
    
    QMenu *menu;
    QAction *newRecordAction;
    QAction *openSaveDirectoryAction;
    QAction *aboutAction;
    QAction *helpAction;
    QAction *exitAction;
};

#endif
