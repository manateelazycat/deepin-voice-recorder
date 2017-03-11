#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QListWidget>
#include <QMenu>
#include <QAction>
#include "file_item.h"

class FileView : public QListWidget
{
    Q_OBJECT
    
public:
    FileView(QWidget *parent=0);
    void mousePressEvent(QMouseEvent *event);
    
public slots:
    void handleCurentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void handleItemClicked(QListWidgetItem *item);
    void handlePlay();
    void handlePause();
    void handleResume();
    void handleStop();
    void handlePlayFinish(QString filepath);
    void onRightClick(QPoint pos);
    void renameItem();
    void deleteItem();
    
signals:
    void play(QString filepath);
    void pause(QString filepath);
    void resume(QString filepath);
    void stop(QString filepath);
    void rightClick(QPoint pos);
    
private:
    QListWidgetItem *currentWidgetItem = 0;
    QListWidgetItem *rightSelectItem = 0;
    
    QMenu *rightMenu;
    QAction *renameAction;
    QAction *deleteAction;
};

#endif
