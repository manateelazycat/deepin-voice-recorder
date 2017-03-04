#ifndef RECORDPAGE_H
#define RECORDPAGE_H

#include <QVBoxLayout>
#include "dimagebutton.h"

#include <QAudioRecorder>

DWIDGET_USE_NAMESPACE

class RecordPage : public QWidget
{
    Q_OBJECT
    
public:
    RecordPage(QWidget *parent = 0);
    DImageButton *recordButton;
    QVBoxLayout *layout;
    
    void startRecord();
    QString getRecordingFilepath();
    
private:
    QLabel *recordTimeLabel;
    QAudioRecorder *audioRecorder;
};

#endif
