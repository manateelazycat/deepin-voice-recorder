#ifndef FILEITEM_H
#define FILEITEM_H

#include <QFileInfo>
#include <QWidget>
#include <QListWidgetItem>
#include "dimagebutton.h"
#include <QHBoxLayout>
#include <QLabel>
#include "line_edit.h"

DWIDGET_USE_NAMESPACE

class FileItem : public QWidget
{
    Q_OBJECT
    
public:
    FileItem(QWidget *parent=0);
    void setFileInfo(QFileInfo info);
    int getDuration();
    QListWidgetItem* getItem();
    
    static const int STATUS_NORMAL;
    static const int STATUS_RENAME;
    static const int STATUS_PLAY;
    static const int STATUS_PLAY_PAUSE;
    static const int STATUS_PAUSE_PLAY;
    
public slots:
    void switchStatus(int status);
    
signals:
    void clickedRenameButton();
    
private:
    QHBoxLayout *layout;
    QHBoxLayout *infoLayout;
    QHBoxLayout *actionLayout;
    
    QLabel *fileIcon;
    QLabel *fileName;
    QLabel *lengthLabel;
    
    LineEdit *lineEdit;
    DImageButton *renameButton;
    DImageButton *showNodeButton;
    DImageButton *playStartButton;
    
    DImageButton *playPauseButton;
    DImageButton *playStopButton;

    DImageButton *pausePlayButton;
    DImageButton *pauseStopButton;

    QWidget *fileDisplayContainer;
    QHBoxLayout *fileDisplayLayout;
    QWidget *fileRenameContainer;
    QHBoxLayout *fileRenameLayout;
    
    QWidget *normalActionContainer;
    QHBoxLayout *normalActionLayout;
    QWidget *playActionContainer;
    QHBoxLayout *playActionLayout;
    QWidget *pausePlayActionContainer;
    QHBoxLayout *pausePlayActionLayout;
    QWidget *playPauseActionContainer;
    QHBoxLayout *playPauseActionLayout;
    
    QFileInfo fileInfo;
    
    QListWidgetItem *item;
};

#endif
