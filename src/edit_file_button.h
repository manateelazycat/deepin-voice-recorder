#ifndef EDITFILEBUTTON_H
#define EDITFILEBUTTON_H

#include <QWidget>
#include <QVBoxLayout>

#include "dimagebutton.h"

DWIDGET_USE_NAMESPACE

class EditFileButton : public QWidget
{
    Q_OBJECT
    
public:
    EditFileButton(QWidget *parent = 0);
    QLabel *lengthLabel;
    
private:
    DImageButton *playButton;
    DImageButton *pauseButton;
    DImageButton *stopButton;
    DImageButton *nodeButton;
    
    QVBoxLayout *layout;
};

#endif
