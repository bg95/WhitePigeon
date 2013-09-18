#ifndef VERSIONDIALOG_H
#define VERSIONDIALOG_H

#include <QDialog>
#include <QString>
#include "ui_versiondialog.h"

class VersionDialog : public QDialog, public Ui::versiondialog
{
	Q_OBJECT

public:
    VersionDialog(QWidget *parent = 0);
    ~VersionDialog();

    int getNumber() const;
    QString getInformation() const;
};

#endif
