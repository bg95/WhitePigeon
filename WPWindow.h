#ifndef WPWINDOW_H
#define WPWINDOW_H

#include <QMdiSubWindow>
#include <QGraphicsScene>   // Deleted
#include <QGraphicsView>    // Deleted

typedef QGraphicsScene musicScene; // class musicScene;
typedef QGraphicsView musicView;   // class musicView;
class WPSynthesisController;
class WPScore;
class QWebView;

class WPWindow : public QMdiSubWindow
{
	Q_OBJECT

public:
    WPWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);

    enum Mode {File, Web};

    Mode getMode() const;
    void setMode(Mode __mode);

    bool isSaved() const;
    QString currentFilePath() const;
    bool loadFile(const QString &file);
    bool saveFile();
    bool saveFile(const QString &file);
    void play_with(WPSynthesisController *controller);

protected:
    void closeEvent(QCloseEvent *closeEvent);

signals:
    void pathModified();

private slots:
    void onScoreModified();
	void changeFilePathInWebMode(const QUrl &url);
	void loadingFailure(const bool &);

private:
    bool okToContinue();

    bool saved;
    Mode mode;
    QString filePath;
    WPScore *score;
    int lastVersion;
    musicView *view;
    musicScene *scene;
    QWebView *webView;
};

#endif // WPWINDOW_H
