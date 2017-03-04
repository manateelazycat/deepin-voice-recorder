#ifndef EDITPAGE_H
#define EDITPAGE_H

#include <QVBoxLayout>
#include "dimagebutton.h"

DWIDGET_USE_NAMESPACE

class EditPage : public QWidget
{
    Q_OBJECT
    
public:
    EditPage(QWidget *parent = 0);
    DImageButton *editButton;
    QVBoxLayout *layout;
};

#endif
