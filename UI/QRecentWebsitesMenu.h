#ifndef QRECENTWEBSITESMENU_H
#define QRECENTWEBSITESMENU_H

#include <QMenu>
#include <QPair>
#include <QList>

typedef QPair<QString, QString> WebHistory;

class QRecentWebsitesMenu : public QMenu
{
    Q_OBJECT
    Q_PROPERTY(int maxCount READ maxCount WRITE setMaxCount)

    friend class MainWindow;

public:
    QRecentWebsitesMenu(QWidget * parent = 0 );
    QRecentWebsitesMenu(const QString & title, QWidget * parent = 0 );

    int maxCount() const;
    QByteArray saveState() const;
    bool restoreState(const QByteArray &state);

public slots:
    void addRecentWebsite(const WebHistory &history);
    void clearMenu();
    void setMaxCount(int);

signals:
    void recentWebsiteTriggered(const QString &);
    void recentWebsitesModified();

private slots:
    void menuTriggered(QAction*);
    void updateRecentWebsiteActions();

private:
    int m_maxCount;
    QList<WebHistory> m_list;
};

#endif // QRECENTWEBSITESMENU_H
