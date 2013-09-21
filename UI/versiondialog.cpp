#include <QtWidgets>

#include "versiondialog.h"

VersionDialog::VersionDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi(this);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
}

VersionDialog::~VersionDialog()
{
}


QString VersionDialog::getInformation() const
{
    return textEdit->toPlainText();
}

int VersionDialog::getNumber() const
{
    return comboBox->currentText().toInt();
}
