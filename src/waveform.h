#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QEvent>
#include <QWidget>
#include <QTimer>
#include <QDateTime>

class Waveform : public QWidget
{
    Q_OBJECT
    
public:
    Waveform(QWidget *parent = 0);
    
public slots:
    void updateWave(float sample);
    void renderWave();
    
protected:
    void paintEvent(QPaintEvent *event);
    
private:
    QList<float> sampleList; 
    QTimer *renderTimer;
    
    QDateTime lastSampleTime;
};

#endif
