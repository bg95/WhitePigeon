#ifndef WPWINDOW_H
#define WPWINDOW_H

#include <QMdiSubWindow>
#include <QGraphicsView>    // Deleted

class musicScene;
typedef QGraphicsView musicView;   // class musicView;
class WPSynthesisController;
class WPScore;

class WPWindow : public QMdiSubWindow
{
public:
    WPWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

    bool isSaved() const;
    QString currentFilePath() const;
    bool loadFile(const QString &file);
    bool saveFile();
    bool saveFile(const QString &file);
    void play_with(WPSynthesisController *controller);

protected:
    void closeEvent(QCloseEvent *closeEvent);

private slots:
    void onScoreModified();

private:
    bool okToContinue();

    bool saved;
    QString filePath;
    WPScore *score;
    int lastVersion;
    musicView *view;
    musicScene *scene;
};

#endif // WPWINDOW_H
