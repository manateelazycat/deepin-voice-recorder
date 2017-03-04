#include <QVBoxLayout>
#include <QStandardPaths>
#include <QDir>
#include <QDate>
#include "record_page.h"
#include "utils.h"

#include <QAudioRecorder>
#include <QUrl>

#include "dimagebutton.h"

DWIDGET_USE_NAMESPACE

RecordPage::RecordPage(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout();
    setLayout(layout);

    recordTimeLabel = new QLabel("00:00");

    recordButton = new DImageButton(
        Utils::getImagePath("record_small_normal.png"),
        Utils::getImagePath("record_small_hover.png"),
        Utils::getImagePath("record_small_press.png")
        );


    layout->addWidget(recordTimeLabel, 0, Qt::AlignCenter);
    layout->addWidget(recordButton, 0, Qt::AlignCenter);

    audioRecorder = new QAudioRecorder(this);
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
