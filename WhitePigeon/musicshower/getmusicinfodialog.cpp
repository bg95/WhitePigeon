#include "getmusicinfodialog.h"

#include <QtWidgets>

getMusicInfoDialog::getMusicInfoDialog(QWidget *parent)
    : QDialog(parent), created(false)
{
    nameLabel = new QLabel(tr("music &name:"));
    nameEdit = new QLineEdit;
    nameLabel->setBuddy(nameEdit);
    doLabel = new QLabel(tr("1="));
    doLabel->setAlignment(Qt::AlignLeft);
    doEdit = new QLineEdit;
    doLabel->setBuddy(doEdit);
    sonEdit = new QLineEdit;
    motherEdit = new QLineEdit;
    divideLabel = new QLabel(tr("/"));

    yesButton = new QPushButton(tr("Yes"));
    yesButton->setDefault(true);
    cancelButton = new QPushButton(tr("Cancel"));
    connect(yesButton, SIGNAL(clicked()), this, SLOT(yesClicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    QHBoxLayout *hlayout1 = new QHBoxLayout;
    hlayout1->addWidget(nameLabel);
    hlayout1->addWidget(nameEdit);
    QHBoxLayout *hlayout2 = new QHBoxLayout;
    hlayout2->addWidget(doLabel);
    hlayout2->addWidget(doEdit);
    QHBoxLayout *hlayout3 = new QHBoxLayout;
    hlayout3->addWidget(sonEdit);
    hlayout3->addWidget(divideLabel);
    hlayout3->addWidget(motherEdit);
    QHBoxLayout *hlayout4 = new QHBoxLayout;
    hlayout4->addWidget(cancelButton);
    hlayout4->addStretch();
    hlayout4->addWidget(yesButton);
    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout1);
    vlayout->addLayout(hlayout2);
    vlayout->addLayout(hlayout3);
    vlayout->addLayout(hlayout4);
    setLayout(vlayout);
    setWindowTitle(tr("New music"));
    //qDebug() << "hello world";

}

void getMusicInfoDialog::yesClicked()
{
    name = nameEdit->text();
    tune = doEdit->text();
    son = sonEdit->text();
    mother = motherEdit->text();
    if (name.isEmpty() || tune.isEmpty() || son.isEmpty() || mother.isEmpty())
    {
        QMessageBox::warning(this, tr("Warning"), tr("uncompleted"));
    }
    else if (tune.length() != 1)
    {
        QMessageBox::warning(this, tr("Bad Input"),
                             tr("the main tune should a character"));
    }
    else
    {
        bool soncanChange;
        son.toInt(&soncanChange);
        bool mothercanChange;
        mother.toInt(&mothercanChange);

        if (soncanChange && mothercanChange)
        {
            created = true;
            close();
        }
        else
        {
            QMessageBox::warning(this, tr("Bad Input"),
                                 tr("the beat should be integer"));
        }

    }

}
