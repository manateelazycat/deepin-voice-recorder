#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QListWidget>

class FileView : public QListWidget
{
    Q_OBJECT
    
public:
    FileView(QListWidget *parent=0);
};

#endif
