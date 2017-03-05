#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QStandardPaths>
#include <QDir>
#include <QDate>
#include <QTime>
#include "record_page.h"
#include "utils.h"

#include <QDebug>

#include <QAudioRecorder>
#include <QAudioProbe>
#include <QUrl>

#include "dimagebutton.h"
#include "waveform.h"

#include "mute_button.h"
#include "recording_button.h"

DWIDGET_USE_NAMESPACE

static qreal getPeakValue(const QAudioFormat &format);
static QVector<qreal> getBufferLevels(const QAudioBuffer &buffer);

template <class T>
static QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels);

RecordPage::RecordPage(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout();
    setLayout(layout);

    titleLabel = new QLabel("新录音");
    QFont titleFont;
    titleFont.setPixelSize(26);
    titleLabel->setFont(titleFont);
    muteButton = new MuteButton();
    waveform = new Waveform();
    QFont recordTimeFont;
    recordTimeFont.setPixelSize(14);
    recordTimeLabel = new QLabel("00:00");
    recordTimeLabel->setFont(recordTimeFont);

    QWidget *buttonWidget = new QWidget();
    buttonWidget->setFixedWidth(440);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonWidget->setLayout(buttonLayout);
    
    newNodeButton = new DImageButton(
        Utils::getImagePath("new_node_normal.png"),
        Utils::getImagePath("new_node_hover.png"),
        Utils::getImagePath("new_node_press.png")
        );

    finishButton = new DImageButton(
        Utils::getImagePath("finish_normal.png"),
        Utils::getImagePath("finish_hover.png"),
        Utils::getImagePath("finish_press.png")
        );

    recordingButton = new RecordingButton();
    
    // FIXME: i don't know how to make hboxlayout at center of vboxlayout.
    buttonLayout->addSpacing(35);
    buttonLayout->addWidget(newNodeButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(recordingButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(finishButton);
    buttonLayout->addSpacing(15);
    
    layout->addSpacing(36);
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    layout->addSpacing(20);
    layout->addWidget(muteButton, 0, Qt::AlignHCenter);
    layout->addSpacing(80);
    layout->addWidget(waveform, 1, Qt::AlignHCenter);
    layout->addSpacing(58);
    layout->addWidget(recordTimeLabel, 0, Qt::AlignHCenter);
    layout->addStretch();
    layout->addWidget(buttonWidget);
    buttonLayout->addSpacing(36);
    
    audioRecorder = new QAudioRecorder(this);
    
    audioProbe = new QAudioProbe(this);
    if (audioProbe->setSource(audioRecorder)) {
        connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(renderLevel(QAudioBuffer)));
        
        connect(this, SIGNAL(updateLevel(float)), waveform, SLOT(updateWave(float)));
    }
    
    startRecord();
    
    connect(finishButton, SIGNAL(clicked()), this, SLOT(stopRecord()));
    connect(recordingButton, SIGNAL(pause()), this, SLOT(pauseRecord()));
    connect(recordingButton, SIGNAL(resume()), this, SLOT(resumeRecord()));
}

void RecordPage::startRecord()
{
    audioRecorder->setOutputLocation(getRecordingFilepath());
    audioRecorder->record();
}

void RecordPage::stopRecord()
{
    audioRecorder->stop();
}

void RecordPage::pauseRecord()
{
    audioRecorder->pause();
}

void RecordPage::resumeRecord()
{
    audioRecorder->record();
}

QString RecordPage::getRecordingFilepath()
{
    QDir musicDirectory = QDir(QStandardPaths::standardLocations(QStandardPaths::MusicLocation).first());
    QString subDirectory = "Deepin Voice Recorder";
    QString recordDirectory = musicDirectory.filePath(subDirectory);
    musicDirectory.mkdir(subDirectory);

    QDateTime date = QDateTime::currentDateTime();

    return QDir(recordDirectory).filePath(QString("%1_%2.wav").arg("deepin-voice-recorder").arg(date.toString("yyyyMMddhhmmss")));
}

// This function returns the maximum possible sample value for a given audio format
qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

// returns the audio level for each channel
QVector<qreal> getBufferLevels(const QAudioBuffer& buffer)
{
    QVector<qreal> values;

    if (!buffer.format().isValid() || buffer.format().byteOrder() != QAudioFormat::LittleEndian)
        return values;

    if (buffer.format().codec() != "audio/pcm")
        return values;

    int channelCount = buffer.format().channelCount();
    values.fill(0, channelCount);
    qreal peak_value = getPeakValue(buffer.format());
    if (qFuzzyCompare(peak_value, qreal(0)))
        return values;

    switch (buffer.format().sampleType()) {
    case QAudioFormat::Unknown:
    case QAudioFormat::UnSignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<quint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<quint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<quint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);
        break;
    case QAudioFormat::Float:
        if (buffer.format().sampleSize() == 32) {
            values = getBufferLevels(buffer.constData<float>(), buffer.frameCount(), channelCount);
            for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;
        }
        break;
    case QAudioFormat::SignedInt:
        if (buffer.format().sampleSize() == 32)
            values = getBufferLevels(buffer.constData<qint32>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 16)
            values = getBufferLevels(buffer.constData<qint16>(), buffer.frameCount(), channelCount);
        if (buffer.format().sampleSize() == 8)
            values = getBufferLevels(buffer.constData<qint8>(), buffer.frameCount(), channelCount);
        for (int i = 0; i < values.size(); ++i)
            values[i] /= peak_value;
        break;
    }

    return values;
}

template <class T>
QVector<qreal> getBufferLevels(const T *buffer, int frames, int channels)
{
    QVector<qreal> max_values;
    max_values.fill(0, channels);

    for (int i = 0; i < frames; ++i) {
        for (int j = 0; j < channels; ++j) {
            qreal value = qAbs(qreal(buffer[i * channels + j]));
            if (value > max_values.at(j))
                max_values.replace(j, value);
        }
    }

    return max_values;
}

void RecordPage::renderLevel(const QAudioBuffer &buffer)
{
    qreal volume = audioRecorder->volume();
    QVector<qreal> levels = getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
        emit updateLevel(volume * levels.at(i));
    }
}
