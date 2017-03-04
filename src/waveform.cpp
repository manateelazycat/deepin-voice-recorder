#include <QWidget>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QDateTime>

#include <QDebug>

#include "waveform.h"

const int Waveform::SAMPLE_DURATION = 30;
const int Waveform::WAVE_WIDTH = 2;
const int Waveform::WAVE_DURATION = 4;

Waveform::Waveform(QWidget *parent) : QWidget(parent)
{
    setFixedSize(350, 70);

    lastSampleTime = QDateTime::currentDateTime();

    renderTimer = new QTimer();
    connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderWave()));
    renderTimer->start(SAMPLE_DURATION);
}

void Waveform::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int volume = 0;
    for (int i = 0; i < sampleList.size(); i++) {
        volume = sampleList[i] * rect().height() * 30;
        
        qDebug() << "******* " << sampleList[i] << volume;
        
        QRect sampleRect(rect().x() + i * WAVE_DURATION, rect().y() + (rect().height() - volume) / 2, WAVE_WIDTH, volume);
        
        QLinearGradient gradient(sampleRect.topLeft(), sampleRect.bottomLeft());
        gradient.setColorAt(0, QColor("#ffbd78"));
        gradient.setColorAt(1, QColor("#ff005c"));
        painter.fillRect(sampleRect, gradient);
    }
}

void Waveform::updateWave(float sample)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    if (lastSampleTime.msecsTo(currentTime) > SAMPLE_DURATION) {
        if (sampleList.size() > rect().width() / WAVE_DURATION) {
            sampleList.pop_front();
        }
        sampleList << sample;

        qDebug() << sample;

        lastSampleTime = currentTime;
    }
}

void Waveform::renderWave()
{
    repaint();
}
