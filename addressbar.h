#ifndef ADDRESSBAR_H
#define ADDRESSBAR_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QMdiSubWindow;

class AddressBar : public QWidget
{
    Q_OBJECT

public:
    explicit AddressBar(QWidget *parent = 0);

signals:
    void go(QString);

public slots:
    void goToSite();
    void showPath(QMdiSubWindow *__window);

private:
    QLabel *label;
    QLineEdit *addressEdit;
    QPushButton *goButton;
    QHBoxLayout *layout;
};

#endif // ADDRESSBAR_H
