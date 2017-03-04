#include <QVBoxLayout>
#include "home_page.h"
#include "utils.h"

#include "dimagebutton.h"

DWIDGET_USE_NAMESPACE

HomePage::HomePage(QWidget *parent) : QWidget(parent)
{
    recordButton = new DImageButton(
        Utils::getImagePath("home_page_record_normal.png"),
        Utils::getImagePath("home_page_record_hover.png"),
        Utils::getImagePath("home_page_record_press.png")
        );

    layout = new QVBoxLayout();
    setLayout(layout);

    layout->addWidget(recordButton, 0, Qt::AlignCenter);
}
