#include <QtWidgets>

#include "addressbar.h"

AddressBar::AddressBar(QWidget *parent)
    : QWidget(parent)
{
    addressEdit = new QLineEdit;
    addressEdit->setParent(parent);

    goButton = new QPushButton;
    goButton->setParent(parent);
    goButton->setText("Go");

    layout = new QHBoxLayout;
    layout->addWidget(addressEdit);
    layout->addWidget(goButton);
    setLayout(layout);

    connect(goButton, SIGNAL(clicked()), this, SLOT(goToSite()));
    connect(addressEdit, SIGNAL(returnPressed()), this, SLOT(goToSite()));
}

void AddressBar::goToSite()
{
    QString address = addressEdit->text();
    emit go(address);
}
