#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dmainwindow.h"

#include "home_page.h"
#include "record_page.h"
#include "list_page.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT
    
    static const int PAGE_TYPE_HOME;
    static const int PAGE_TYPE_RECORD;
    static const int PAGE_TYPE_LIST;
    
public:
    MainWindow(DMainWindow *parent = 0);
    
public slots:
    void showHomePage();
    void showRecordPage();
    void showListPage(QString recordingPath);
    
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
    
    QMenu *menu;
    QAction *newRecordAction;
    QAction *openSaveDirectoryAction;
    QAction *aboutAction;
    QAction *helpAction;
    QAction *quitAction;
    
    int pageType;
};

#endif
