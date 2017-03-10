#ifndef WAVEFORM_H
#define WAVEFORM_H

#include <QEvent>
#include <QWidget>
#include <QTimer>
#include <QDateTime>

#include <QAudioProbe>

class Waveform : public QWidget
{
    Q_OBJECT

    static const int SAMPLE_DURATION;
    static const int WAVE_WIDTH;
    static const int WAVE_DURATION;

public:
    Waveform(QWidget *parent = 0);
    
    static qreal getPeakValue(const QAudioFormat &format);
    static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

    template <class T>
    static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);
                                                                                    
    void clearWave();
                                                                                    
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
