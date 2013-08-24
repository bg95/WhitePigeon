#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/* Class declearation */
class QLabel;
class OscilloscopeWindow;
class QRecentFilesMenu;
class QMdiArea;
class QMdiSubWindow;
class WPWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    /* Action functions */
    void newFile();
    void loadFile();
    void loadFile(const QString &file);
    void showOscilloscope();
    void updateStatusBar(QMdiSubWindow *window);

private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();

    WPWindow *createNewChild();
    WPWindow *findChild(const QString &file);

    /* Action list */
    QAction *newAction;
    QAction *openAction;
    QAction *oscilloscopeAction;

    /* Menu list */
    QMenu *fileMenu;
    QRecentFilesMenu *recentFilesMenu;
    QMenu *toolsMenu;

    /* ToolBar list */
    QToolBar *fileToolBar;
    QToolBar *toolBar;

    /* Private widget list */
    QMdiArea *mdiArea;
    OscilloscopeWindow *oscilloscopeWindow;
    int countNumber;
};

#endif
