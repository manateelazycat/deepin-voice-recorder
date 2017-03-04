#include <QWidget>
#include <QEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QTimer>
#include <QDateTime>

#include <QDebug>

#include "waveform.h"

Waveform::Waveform(QWidget *parent) : QWidget(parent)
{
    setFixedHeight(100);

    lastSampleTime = QDateTime::currentDateTime();

    renderTimer = new QTimer();
    connect(renderTimer, SIGNAL(timeout()), this, SLOT(renderWave()));
    renderTimer->start(30);
}

void Waveform::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    int volume = 0;
    for (int i = 0; i < sampleList.size(); i++) {
        volume = sampleList[i] * rect().height() / 300;
        QRect sampleRect(rect().x() + i * 4, rect().y() + (rect().height() - volume) / 2, 2, volume);
        
        QLinearGradient gradient(sampleRect.topLeft(), sampleRect.bottomLeft());
        gradient.setColorAt(0, QColor("#ffbd78"));
        gradient.setColorAt(1, QColor("#ff005c"));
        painter.fillRect(sampleRect, gradient);
    }
}

void Waveform::updateWave(float sample)
{
    QDateTime currentTime = QDateTime::currentDateTime();

    if (lastSampleTime.msecsTo(currentTime) > 30) {
        if (sampleList.size() > rect().width() / 4) {
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
