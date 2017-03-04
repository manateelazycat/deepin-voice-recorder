#include <QVBoxLayout>
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

DWIDGET_USE_NAMESPACE

RecordPage::RecordPage(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout();
    setLayout(layout);

    waveform = new Waveform();
    
    qDebug() << waveform->geometry();
    
    recordTimeLabel = new QLabel("00:00");

    recordButton = new DImageButton(
        Utils::getImagePath("record_small_normal.png"),
        Utils::getImagePath("record_small_hover.png"),
        Utils::getImagePath("record_small_press.png")
        );


    layout->addWidget(waveform);
    layout->addWidget(recordTimeLabel, 0, Qt::AlignCenter);
    layout->addWidget(recordButton, 0, Qt::AlignCenter);
    
    audioRecorder = new QAudioRecorder(this);
    
    audioProbe = new QAudioProbe(this);
    if (audioProbe->setSource(audioRecorder)) {
        connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(renderLevel(QAudioBuffer)));
        
        connect(this, SIGNAL(updateLevel(float)), waveform, SLOT(updateWave(float)));
    }
    
    startRecord();
}

void RecordPage::startRecord()
{
    audioRecorder->setOutputLocation(getRecordingFilepath());
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

void RecordPage::renderLevel(const QAudioBuffer &buffer)
{
    // qDebug() << volume;
    // const float sample = qAbs(*reinterpret_cast<const float*>(buffer.constData()));
    const quint8 *data = buffer.constData<quint8>();
    quint8 value = *data;    
    
    emit updateLevel(value);
}
