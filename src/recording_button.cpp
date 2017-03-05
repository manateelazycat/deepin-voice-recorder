#include <QVBoxLayout>
#include "dimagebutton.h"
#include "recording_button.h"
#include "utils.h"

#include <QDebug>

DWIDGET_USE_NAMESPACE

RecordingButton::RecordingButton(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout();
    setLayout(layout);
    
    pauseButton = new DImageButton(
        Utils::getImagePath("record_pause_normal.png"),
        Utils::getImagePath("record_pause_hover.png"),
        Utils::getImagePath("record_pause_press.png")
        );
    connect(pauseButton, SIGNAL(clicked()), this, SLOT(handlePause()));

    resumeButton = new DImageButton(
        Utils::getImagePath("record_resume_normal.png"),
        Utils::getImagePath("record_resume_hover.png"),
        Utils::getImagePath("record_resume_press.png")
        );
    connect(resumeButton, SIGNAL(clicked()), this, SLOT(handleResume()));
    
    layout->addWidget(pauseButton, 0, Qt::AlignCenter);
}

void RecordingButton::handlePause() {
    layout->removeWidget(pauseButton);
    pauseButton->setParent(NULL);
    layout->addWidget(resumeButton, 0, Qt::AlignCenter);
    emit pause();
}

void RecordingButton::handleResume() {
    layout->removeWidget(resumeButton);
    resumeButton->setParent(NULL);
    layout->addWidget(pauseButton, 0, Qt::AlignCenter);
    emit resume();
}
