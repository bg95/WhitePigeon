#include "QRecentWebsitesMenu.h"

static const qint32 RecentWebsitesMenuMagic = 0xef;

QRecentWebsitesMenu::QRecentWebsitesMenu(QWidget * parent)
: QMenu(parent),
  m_maxCount(50)
{
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));

    setMaxCount(m_maxCount);
}

QRecentWebsitesMenu::QRecentWebsitesMenu(const QString & title, QWidget * parent)
: QMenu(title, parent),
  m_maxCount(50)
{
    connect(this, SIGNAL(triggered(QAction*)), this, SLOT(menuTriggered(QAction*)));

    setMaxCount(m_maxCount);
}

void QRecentWebsitesMenu::addRecentWebsite(const WebHistory &history)
{
    for (QList<WebHistory>::Iterator iter = m_list.begin();
         iter != m_list.end();)
    {
        if (iter->first == history.first)
        {
            iter = m_list.erase(iter);
        }
        else
        {
            ++iter;
        }
    }
    m_list.prepend(history);

    while (m_list.size() > maxCount())
    {
        m_list.removeLast();
    }

    updateRecentWebsiteActions();
    emit recentWebsitesModified();
}

void QRecentWebsitesMenu::clearMenu()
{
    m_list.clear();

    updateRecentWebsiteActions();
    emit recentWebsitesModified();
}

int QRecentWebsitesMenu::maxCount() const
{
    return m_maxCount;
}

QByteArray QRecentWebsitesMenu::saveState() const
{
    int version = 0;
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);

    stream << qint32(RecentWebsitesMenuMagic);
    stream << qint32(version);
    stream << m_list;

    return data;
}

bool QRecentWebsitesMenu::restoreState(const QByteArray &state)
{
    int version = 0;
    QByteArray sd = state;
    QDataStream stream(&sd, QIODevice::ReadOnly);
    qint32 marker;
    qint32 v;

    stream >> marker;
    stream >> v;
    if (marker != RecentWebsitesMenuMagic || v != version)
        return false;

    stream >> m_list;

    updateRecentWebsiteActions();

    return true;
}

void QRecentWebsitesMenu::setMaxCount(int count)
{
    m_maxCount = count;

    updateRecentWebsiteActions();
}

void QRecentWebsitesMenu::menuTriggered(QAction* action)
{
    if (action->data().isValid())
        emit recentWebsiteTriggered(action->data().toString());
}

void QRecentWebsitesMenu::updateRecentWebsiteActions()
{
    clear();

    int i = 0;
    for (QList<WebHistory>::Iterator iter = m_list.begin();
         iter != m_list.end();
         ++iter)
    {
        QAction *recentWebsiteAct = addAction(tr("%1 %2").arg(++i).arg(iter->second));
        recentWebsiteAct->setData(iter->first);
    }
    addSeparator();
    addAction(tr("Clear Menu"), this, SLOT(clearMenu()));

    setEnabled(i > 0);
}
