#ifndef RECORDPAGE_H
#define RECORDPAGE_H

#include <QVBoxLayout>
#include "dimagebutton.h"

#include <QAudioRecorder>
#include <QAudioProbe>

#include "mute_button.h"
#include "waveform.h"

#include "recording_button.h"

DWIDGET_USE_NAMESPACE

class RecordPage : public QWidget
{
    Q_OBJECT
    
public:
    RecordPage(QWidget *parent = 0);
    
    RecordingButton *recordingButton;
    QVBoxLayout *layout;
    DImageButton *newNodeButton;
    DImageButton *finishButton;
    MuteButton *muteButton;
    
    QString getRecordingFilepath();
    
signals:
    void updateLevel(float level);
    
public slots:
    void renderLevel(const QAudioBuffer &buffer);
    void startRecord();
    void stopRecord();
    void pauseRecord();
    void resumeRecord();
    
private:
    QAudioRecorder *audioRecorder;
    QAudioProbe *audioProbe;
    
    Waveform *waveform;
    QLabel *recordTimeLabel;
    QLabel *titleLabel;
};

#endif
