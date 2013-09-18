#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/* Class declearation */
class OscilloscopeWindow;
class QRecentFilesMenu;
class QMdiArea;
class QMdiSubWindow;
class WPWindow;
class WPSynthesisController;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    /* Action functions */
    void newFile();
    void loadFile();
    void loadFile(const QString &file);
    void goToSite();
    void openAWebPage(const QUrl &);
    void setAddressVisible(bool visible);
    bool saveFile();
    bool saveAsFile();
    void closeFile();
    void closeAllFiles();
    void play();
    void stopAll();
    void updateActionsNeedingSubWindow();
    void updateAddressBar();
    void onLoadFinished();
    void showLoadingProgress(int);
    void showStatusBarMessage(const QString &);
    void showOscilloscope();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createAddressBar();
    void createContextMenu();
    void readSettings();
    void writeSettings();

    WPWindow *createNewChild();
    WPWindow *findChild(const QString &file);

    /* Action list */
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;
    QAction *closeAllAction;
    QAction *exitAction;
    QAction *playAction;
    QAction *stopAction;
    QAction *fileToolViewAction;
    QAction *musicToolViewAction;
    QAction *toolToolViewAction;
    QAction *addressViewAction;
    QAction *oscilloscopeAction;

    /* Menu list */
    QMenu *fileMenu;
    QRecentFilesMenu *recentFilesMenu;
    QMenu *musicMenu;
    QMenu *viewMenu;
    QMenu *toolsViewMenu;
    QMenu *toolsMenu;

    /* ToolBar list */
    QToolBar *fileToolBar;
    QToolBar *musicToolBar;
    QToolBar *toolToolBar;

    /* Private widget list */
    QLineEdit *addressEdit;
    QPushButton *goButton;
    QHBoxLayout *addressLayout;
    QMdiArea *mdiArea;
    QVBoxLayout *mainLayout;
    QWidget *widget;
    WPSynthesisController *controller;
    OscilloscopeWindow *oscilloscopeWindow;
    int countNumber;
};

#endif
