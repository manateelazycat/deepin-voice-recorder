#ifndef RECORDPAGE_H
#define RECORDPAGE_H

#include <QVBoxLayout>
#include "dimagebutton.h"

#include <QAudioRecorder>
#include <QAudioProbe>

#include "waveform.h"

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
                                  
signals:
    void updateLevel(float level);
    
public slots:
    void renderLevel(const QAudioBuffer &buffer);
    
private:
    QLabel *recordTimeLabel;
    QAudioRecorder *audioRecorder;
    QAudioProbe *audioProbe;
    
    Waveform *waveform;
};

#endif
