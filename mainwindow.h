#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

/* Class declearation */
class QLabel;
class OscilloscopeWindow;

class musicScene;
class QGraphicsView;
class QToolBar;

class QRecentFilesMenu;
class QMdiArea;
class QMdiSubWindow;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    /* Action functions */
    void newFile();
    void showOscilloscope();

    //void getMusicInformation();

    void updateStatusBar(QMdiSubWindow* window);


private:
    void createActions();
    void createMenus();
    void createToolBar();
    void createStatusBar();
    //void drawMusic();
    void readSettings();
    void writeSettings();
    //void showMusic();

    /* Action list */
    QAction *newAction;
    QAction *oscilloscopeAction;

    /* Menu list */
    QRecentFilesMenu *recentFilesMenu;
    QMenu *toolsMenu;
    QMenu *fileMenu;

    /* ToolBar list */
    QToolBar *fileToolBar;
    QToolBar *toolBar;

    /* Private widget list */
    QMdiArea *mdiArea;
    OscilloscopeWindow *oscilloscopeWindow;
    musicScene *scene;
    QGraphicsView *view;
    int countNumber;
};

#endif
