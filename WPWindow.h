#ifndef WPWINDOW_H
#define WPWINDOW_H

#include <QMdiSubWindow>
#include <QGraphicsScene>   // Deleted
#include <QGraphicsView>    // Deleted

typedef QGraphicsScene WPGraphicsScene; // class WPGraphicsScene;
typedef QGraphicsView WPGraphicsView;   // class WPGraphicsView;
class WPSynthesisController;

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

private:
    bool saved;
    QString filePath;
    WPGraphicsView *view;
    WPGraphicsScene *scene;
};

#endif // WPWINDOW_H
