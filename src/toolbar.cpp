#include <QHBoxLayout>
#include <QLabel>
#include "utils.h"
#include "toolbar.h"
#include <QDebug>
#include <QPushButton>

Toolbar::Toolbar(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    
    QPixmap iconPixmap = QPixmap(Utils::getQrcPath("logo.png"));
    QLabel *iconLabel = new QLabel();
    iconLabel->setPixmap(iconPixmap);
    
    layout->addWidget(iconLabel);
}                                    
