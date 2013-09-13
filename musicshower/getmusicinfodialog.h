#ifndef GETMUSICINFODIALOG_H
#define GETMUSICINFODIALOG_H

#include <QDialog>

class QLabel;
class QLineEdit;
class QPushButton;

class getMusicInfoDialog : public QDialog
{
    Q_OBJECT

public:
    getMusicInfoDialog(QWidget *parent = 0);
    bool created;
    QString name;
    QString tune;
    QString son;
    QString mother;

private slots:
    void yesClicked();

private:
    QLabel *nameLabel;
    QLabel *doLabel;
    QLabel *divideLabel;
    QLineEdit *nameEdit;
    QLineEdit *doEdit;
    QLineEdit *sonEdit;
    QLineEdit *motherEdit;
    QPushButton *yesButton;
    QPushButton *cancelButton;

};

#endif // GETMUSICINFODIALOG_H
