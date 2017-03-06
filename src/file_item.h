#ifndef FILEITEM_H
#define FILEITEM_H

#include <QFileInfo>
#include <QWidget>
#include <QListWidgetItem>
#include "dimagebutton.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>

DWIDGET_USE_NAMESPACE

class FileItem : public QWidget
{
    Q_OBJECT
    
public:
    FileItem(QWidget *parent=0);
    void setFileInfo(QFileInfo info);
    
private:
    QHBoxLayout *layout;
    QHBoxLayout *infoLayout;
    QHBoxLayout *actionLayout;
    
    QLabel *fileIcon;
    QLabel *fileName;
    QLabel *lengthLabel;
    
    QLineEdit *lineEdit;
    DImageButton *renameButton;
    DImageButton *showNodeButton;
    DImageButton *playStartButton;
    
    DImageButton *playPauseButton;
    DImageButton *playStopButton;
    
    QFileInfo fileInfo;
};

#endif
