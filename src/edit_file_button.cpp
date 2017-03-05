#include "edit_file_button.h"

EditFileButton::EditFileButton(QWidget *parent) : QWidget(parent)
{
    layout = new QVBoxLayout(this);
    lengthLabel = new QLabel("10:12");
    
    layout->addWidget(lengthLabel);
}
