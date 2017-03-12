/* -*- Mode: C++; indent-tabs-mode: nil; tab-width: 4 -*-
 * -*- coding: utf-8 -*-
 *
 * Copyright (C) 2011 ~ 2017 Deepin, Inc.
 *               2011 ~ 2017 Wang Yong
 *
 * Author:     Wang Yong <wangyong@deepin.com>
 * Maintainer: Wang Yong <wangyong@deepin.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */ 

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
    void selectItemWithPath(QString path);
    
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
    void displayItem();
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
    QAction *displayAction;
    QAction *deleteAction;
};

#endif
