#include <QVBoxLayout>
#include "edit_page.h"
#include "utils.h"

#include "dimagebutton.h"

DWIDGET_USE_NAMESPACE

EditPage::EditPage(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout();
    setLayout(layout);

    editButton = new DImageButton(
        Utils::getImagePath("record_small_normal.png"),
        Utils::getImagePath("record_small_hover.png"),
        Utils::getImagePath("record_small_press.png")
        );
    
    layout->addWidget(editButton, 0, Qt::AlignCenter);
}
