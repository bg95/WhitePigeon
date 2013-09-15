#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/* Class declearation */
class OscilloscopeWindow;
class QRecentFilesMenu;
class QRecentWebsitesMenu;
class QMdiArea;
class QMdiSubWindow;
class WPWindow;
//class WPSynthesisController;
class QLineEdit;
class QPushButton;
class QHBoxLayout;
class QVBoxLayout;
class QCompleter;

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
    void newPart();
    void undo();
    void redo();
    void switchVersion(QAction*);
    void recordCurrentVersion();
    void refresh();
    void play();
    void stopAll();
    void updateActions();
    void updateVersionMenu(WPWindow *);
    void updateAddressBar();
    void updateCompleter();
    void onLoadFinished(const QPair<QString, QString> &);
    void showLoadingProgress(int);
    void showStatusBarMessage(const QString &);
    void showOscilloscope();
    void recongiseImage();

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createAddressBar();
    void createContextMenu();
    void readSettings();
    void writeSettings();

    WPWindow *createNewChildFileMode();
    WPWindow *createNewChildWebMode();
    WPWindow *findChild(const QString &file);

    /* Action list */
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *closeAction;
    QAction *closeAllAction;
    QAction *exitAction;
    QAction *newPartAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *refreshAction;
    QAction *playAction;
    QAction *stopAction;
    QAction *fileToolViewAction;
    QAction *musicToolViewAction;
    QAction *toolToolViewAction;
    QAction *addressViewAction;
    QAction *oscilloscopeAction;
    QAction *imageRecogniseAction;

    /* Menu list */
    QMenu *fileMenu;
    QRecentFilesMenu *recentFilesMenu;
    QRecentWebsitesMenu *recentWebsitesMenu;
    QMenu *editMenu;
    QMenu *versionMenu;
    QMenu *musicMenu;
    QMenu *viewMenu;
    QMenu *toolsViewMenu;
    QMenu *toolsMenu;

    /* ToolBar list */
    QToolBar *fileToolBar;
    QToolBar *musicToolBar;
    QToolBar *toolToolBar;

    /* Private widget list */
    QPushButton *undoButton;
    QPushButton *redoButton;
    QPushButton *refreshButton;
    QLineEdit *addressEdit;
    QCompleter *completer;
    QPushButton *goButton;
    QHBoxLayout *addressLayout;
    QMdiArea *mdiArea;
    QVBoxLayout *mainLayout;
    QWidget *widget;
    //WPSynthesisController *controller;
    OscilloscopeWindow *oscilloscopeWindow;
    int countNumber;
};

#endif
