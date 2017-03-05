#include <QWidget>
#include <QImage>
#include "mute_button.h"
#include "utils.h"

MuteButton::MuteButton(QWidget *parent) : QWidget(parent)
{
    isMute = false;
    
    muteOnImg = QImage(Utils::getImagePath("mute_on.png"));
    muteOffImg = QImage(Utils::getImagePath("mute_off.png"));
    
    setFixedSize(muteOnImg.width(), muteOnImg.height());
}

void MuteButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setOpacity(1);
    
    if (isMute) {
        painter.drawImage(QPoint(0, 0), muteOffImg);    
    } else {
        painter.drawImage(QPoint(0, 0), muteOnImg);    
    }
}
