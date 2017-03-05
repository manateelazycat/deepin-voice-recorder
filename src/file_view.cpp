#include "file_view.h"
#include <QStandardItemModel>
#include <QFileInfoList>
#include <QDir>
#include <QHeaderView>

#include "line_edit_delegate.h"
#include "icon_delegate.h"
#include "edit_file_button_delegate.h"
#include "utils.h"

FileView::FileView(QTableView *parent) : QTableView(parent)
{
    QStringList filters;
    filters << "*.wav";
    QFileInfoList fileInfoList = QDir("/home/andy/Music/Deepin Voice Recorder").entryInfoList(filters, QDir::Files|QDir::NoDotAndDotDot);

    model = new QStandardItemModel();
    this->setModel(model);
    this->viewport()->setAttribute(Qt::WA_Hover, true);
    this->viewport()->setMouseTracking(true);

    QHeaderView *verticalHeader = this->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(60);

    this->horizontalHeader()->hide();
    this->verticalHeader()->hide();
    this->setShowGrid(false);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setFrameStyle(QFrame::NoFrame);

    horizontalHeader()->setSectionResizeMode (QHeaderView::Stretch);
    setColumnWidth(0,40);
    setColumnWidth(1,300);
    setColumnWidth(2,80);

    IconDelegate* iconDelegate = new IconDelegate();
    this->setItemDelegateForColumn(0, iconDelegate);

    ItemDelegate* delegate = new ItemDelegate();
    this->setItemDelegateForColumn(1, delegate);

    EditFileButtonDelegate* editDelegate = new EditFileButtonDelegate();
    this->setItemDelegateForColumn(2, editDelegate);

    foreach (auto fileInfo, fileInfoList) {
        QList<QStandardItem *> rowData;
        rowData << new QStandardItem(Utils::getImagePath("file.png"));
        rowData << new QStandardItem(fileInfo.baseName());
        rowData << new QStandardItem("00:12");
        model->appendRow(rowData);
    }

    setFixedSize(400, 400);
}
