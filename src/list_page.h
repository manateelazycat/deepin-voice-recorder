#ifndef LISTPAGE_H
#define LISTPAGE_H

#include <QVBoxLayout>
#include "dimagebutton.h"

DWIDGET_USE_NAMESPACE

class ListPage : public QWidget
{
    Q_OBJECT
    
public:
    ListPage(QWidget *parent = 0);
    DImageButton *recordButton;
    QVBoxLayout *layout;
};

#endif
