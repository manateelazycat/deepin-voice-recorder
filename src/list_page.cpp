#include <QVBoxLayout>
#include "list_page.h"
#include "utils.h"

#include "dimagebutton.h"

#include <QDebug>
#include <QMediaPlayer>
#include <QAudioProbe>

DWIDGET_USE_NAMESPACE

ListPage::ListPage(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout();
    setLayout(layout);

    fileView = new FileView();
    connect(fileView, &FileView::play, this, &ListPage::play);
    connect(fileView, &FileView::pause, this, &ListPage::pause);
    connect(fileView, &FileView::resume, this, &ListPage::resume);
    connect(fileView, &FileView::stop, this, &ListPage::stop);

    audioPlayer = new QMediaPlayer();
    connect(audioPlayer, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(handleStateChanged(QMediaPlayer::State)));
    audioProbe = new QAudioProbe();
    if (audioProbe->setSource(audioPlayer)) {
        connect(audioProbe, SIGNAL(audioBufferProbed(QAudioBuffer)), this, SLOT(renderLevel(QAudioBuffer)));
    }

    waveform = new Waveform();
    waveform->hide();
    recordButton = new DImageButton(
        Utils::getImagePath("record_small_normal.png"),
        Utils::getImagePath("record_small_hover.png"),
        Utils::getImagePath("record_small_press.png")
        );

    layout->addWidget(fileView, 0, Qt::AlignCenter);
    layout->addStretch();
    layout->addWidget(waveform, 0, Qt::AlignHCenter);
    layout->addWidget(recordButton, 0, Qt::AlignHCenter);
    layout->addSpacing(28);
}

void ListPage::play(QString filepath)
{
    waveform->show();
    audioPlayer->setMedia(QUrl::fromLocalFile(filepath));
    audioPlayer->play();

    qDebug() << "play " << filepath;
}

void ListPage::pause(QString filepath)
{
    audioPlayer->pause();
    qDebug() << "pause " << filepath;
}

void ListPage::resume(QString filepath)
{
    audioPlayer->play();
    qDebug() << "resume " << filepath;
}

void ListPage::stop(QString filepath)
{
    audioPlayer->stop();
    qDebug() << "stop " << filepath;
}

void ListPage::renderLevel(const QAudioBuffer &buffer)
{
    qreal volume = audioPlayer->volume();
    QVector<qreal> levels = Waveform::getBufferLevels(buffer);
    for (int i = 0; i < levels.count(); ++i) {
        waveform->updateWave(volume * levels.at(i));
    }
}

void ListPage::handleStateChanged(QMediaPlayer::State state)
{
    if (state == QMediaPlayer::StoppedState) {
        waveform->hide();
        waveform->clearWave();
    }
}
