#ifndef FILEVIEW_H
#define FILEVIEW_H

#include <QTableView>
#include <QStandardItemModel>

class FileView : public QTableView
{
    Q_OBJECT
    
public:
    FileView(QTableView *parent=0);
    
private:
    QStandardItemModel *model;
};

#endif
