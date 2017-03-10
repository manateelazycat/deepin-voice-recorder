#ifndef LISTPAGE_H
#define LISTPAGE_H

#include <QVBoxLayout>
#include "dimagebutton.h"
#include "file_view.h"
#include "waveform.h"

#include <QMediaPlayer>
#include <QAudioProbe>

DWIDGET_USE_NAMESPACE

class ListPage : public QWidget
{
    Q_OBJECT
    
public:
    ListPage(QWidget *parent = 0);
    DImageButton *recordButton;
    QVBoxLayout *layout;
    
public slots:
    void play(QString filepath);
    void pause(QString filepath);
    void resume(QString filepath);
    void stop(QString filepath);
    void renderLevel(const QAudioBuffer &buffer);
    void handleStateChanged(QMediaPlayer::State state);
    
private:    
    FileView *fileView;
    Waveform *waveform;
    QMediaPlayer *audioPlayer;
    QAudioProbe *audioProbe;
};

#endif
