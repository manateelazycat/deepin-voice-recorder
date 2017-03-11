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

#include "recording_button.h"

DWIDGET_USE_NAMESPACE

RecordPage::RecordPage(QWidget *parent) : QWidget(parent)
{
    recordingTime = 0;
    
    layout = new QVBoxLayout();
    setLayout(layout);

    titleLabel = new QLabel("新录音");
    QFont titleFont;
    titleFont.setPixelSize(26);
    titleLabel->setFont(titleFont);
    waveform = new Waveform();
    QFont recordTimeFont;
    recordTimeFont.setPixelSize(14);
    recordTimeLabel = new QLabel("00:00");
    recordTimeLabel->setFont(recordTimeFont);

    QWidget *buttonWidget = new QWidget();
    buttonWidget->setFixedWidth(440);
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonWidget->setLayout(buttonLayout);
    
    finishButton = new DImageButton(
        Utils::getImagePath("finish_normal.png"),
        Utils::getImagePath("finish_hover.png"),
        Utils::getImagePath("finish_press.png")
        );

    recordingButton = new RecordingButton();
    
    // FIXME: i don't know how to make hboxlayout at center of vboxlayout.
    buttonLayout->addSpacing(35);
    buttonLayout->addStretch();
    buttonLayout->addWidget(recordingButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(finishButton);
    buttonLayout->addSpacing(15);
    
    layout->addSpacing(36);
    layout->addWidget(titleLabel, 0, Qt::AlignHCenter);
    layout->addSpacing(100);
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
    }
    
    tickerTimer = new QTimer();
    connect(tickerTimer, SIGNAL(timeout()), this, SLOT(renderRecordingTime()));
    tickerTimer->start(1000);
    
    startRecord();
    
    connect(finishButton, SIGNAL(clicked()), this, SLOT(stopRecord()));
    connect(recordingButton, SIGNAL(pause()), this, SLOT(pauseRecord()));
    connect(recordingButton, SIGNAL(resume()), this, SLOT(resumeRecord()));
}

void RecordPage::renderRecordingTime()
{
    if (audioRecorder->state() != QMediaRecorder::StoppedState) {
        recordTimeLabel->setText(Utils::formatMillisecond(recordingTime));
    }
}

void RecordPage::startRecord()
{
    audioRecorder->setOutputLocation(getRecordingFilepath());
    
    QDateTime currentTime = QDateTime::currentDateTime();
    lastUpdateTime = currentTime;
    audioRecorder->record();
}

void RecordPage::stopRecord()
{
    audioRecorder->stop();
    tickerTimer->stop();
}

void RecordPage::pauseRecord()
{
    audioRecorder->pause();
}

void RecordPage::resumeRecord()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    lastUpdateTime = currentTime;
    
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
    QDateTime currentTime = QDateTime::currentDateTime();
    recordingTime += lastUpdateTime.msecsTo(currentTime);
    lastUpdateTime = currentTime;
    
    qreal volume = audioRecorder->volume();
    QVector<qreal> levels = Waveform::getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
        waveform->updateWave(volume * levels.at(i));
    }
}
