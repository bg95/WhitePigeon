#include <QtWidgets>
#include "WPWindow.h"

#include "addressbar.h"

AddressBar::AddressBar(QWidget *parent)
    : QWidget(parent)
{
    label = new QLabel(tr("Address:"));

    addressEdit = new QLineEdit;
    addressEdit->setParent(parent);

    goButton = new QPushButton;
    goButton->setParent(parent);
    goButton->setText("Go");

    layout = new QHBoxLayout;
    layout->addWidget(label);
    layout->addWidget(addressEdit);
    layout->addWidget(goButton);
    setLayout(layout);
    setMaximumHeight(37);

    connect(goButton, SIGNAL(clicked()), this, SLOT(goToSite()));
    connect(addressEdit, SIGNAL(returnPressed()), this, SLOT(goToSite()));
}

void AddressBar::goToSite()
{
    QString address = addressEdit->text();
    emit go(address);
}

void AddressBar::showPath(QMdiSubWindow *__window)
{
    if (__window)
    {
        WPWindow *window = dynamic_cast<WPWindow *>(__window);
        addressEdit->setText(window->currentFilePath());
    }
}
