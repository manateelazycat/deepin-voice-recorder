#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "dmainwindow.h"

#include "home_page.h"
#include "record_page.h"

DWIDGET_USE_NAMESPACE

class MainWindow : public DMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(DMainWindow *parent = 0);
    
public slots:
    void showRecordPage();
    
private:
    HomePage *homePage;
    RecordPage *recordPage;
};

#endif
