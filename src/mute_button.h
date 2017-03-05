#ifndef MUTEBUTTON_H
#define MUTEBUTTON_H

#include <QWidget>
#include <QImage>

class MuteButton : public QWidget
{
    Q_OBJECT
    
public:
    MuteButton(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
                                        
private:
    QImage muteOnImg;
    QImage muteOffImg;

    bool isMute;
};

#endif
