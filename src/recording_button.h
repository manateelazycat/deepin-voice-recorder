#ifndef RECORDINGBUTTON_H
#define RECORDINGBUTTON_H

#include <QVBoxLayout>
#include "dimagebutton.h"

DWIDGET_USE_NAMESPACE

class RecordingButton : public QWidget
{
    Q_OBJECT
    
public:
    RecordingButton(QWidget *parent = 0);
                                        
signals:
    void pause();
    void resume();
    
public slots:
    void handlePause();
    void handleResume();
    
private:
    DImageButton *pauseButton;
    DImageButton *resumeButton;
    
    QVBoxLayout *layout;
};

#endif
