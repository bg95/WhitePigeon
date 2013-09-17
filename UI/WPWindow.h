#ifndef WPWINDOW_H
#define WPWINDOW_H

#include <QMdiSubWindow>

class musicScene;
class musicView;
//class WPSynthesisController;
class WPScore;
class QWebView;
class QVersionMenu;

class WPWindow : public QMdiSubWindow
{
	Q_OBJECT

    friend class MainWindow;

public:
    enum Mode {File, Web};

    WPWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0);
    WPWindow(Mode __mode, QWidget *parent = 0, Qt::WindowFlags flags = 0);
    ~WPWindow();

    Mode getMode() const;
    void setMode(Mode __mode);

    bool isSaved() const;
    QString currentFilePath() const;
    bool loadFile(const QString &file);
    bool saveFile();
    bool saveFile(const QString &file);
    void newPart();
    void undo();
    void redo();
    void switchVersion(int version);
    void refresh();
    void play_with();

protected:
    void closeEvent(QCloseEvent *closeEvent);

signals:
    void pathModified();
    void loadProgress(int);
    void statusBarMessage(const QString &);
    void loadFinished(const QPair<QString, QString> &);
    void linkClicked(const QUrl &);

private slots:
	void changeFilePathInWebMode(const QUrl &url);
    void onLoadProgress(int progress);
    void onStatusBarMessage(const QString &);
    void onLinkClicked(const QUrl &);
    void loadFailure(bool);
    void refreshIcon();

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
    int version[5];
    bool enable[5];
    QString tip[5];
};

#endif // WPWINDOW_H
