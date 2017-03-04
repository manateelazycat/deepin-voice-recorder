#include <QVBoxLayout>
#include "record_page.h"
#include "utils.h"

#include "dimagebutton.h"

DWIDGET_USE_NAMESPACE

RecordPage::RecordPage(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout();
    setLayout(layout);

    recordTimeLabel = new QLabel("00:00");
    
    recordButton = new DImageButton(
        Utils::getImagePath("record_page_record_normal.png"),
        Utils::getImagePath("record_page_record_hover.png"),
        Utils::getImagePath("record_page_record_press.png")
        );
    

    layout->addWidget(recordTimeLabel, 0, Qt::AlignCenter);
    layout->addWidget(recordButton, 0, Qt::AlignCenter);
}
