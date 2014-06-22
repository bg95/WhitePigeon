#include "WPDLLTimbreManager.h"

std::string WPDLLTimbreManager::TimbrePluginDir("/home/pt-cr/Projects/WhitePigeon/WhitePigeon/plugins/");

WPDLLTimbreManager::WPDLLTimbreManager()
{
}

WPDLLTimbreManager::~WPDLLTimbreManager()
{
    clearTimbres();
}

bool WPDLLTimbreManager::setTimbre(std::string timbrenamepara)
{
    std::istringstream iss(timbrenamepara);
    char c;
    name.clear();
    para.clear();
    fname.clear();
    while ((c = iss.get()) != -1)
    {
        if (c == ' ')
            break;
        name.push_back(c);
    }
    iss >> para;
    fname.append(TimbrePluginDir);
    fname.append("lib");
    fname.append(name);
    fname.append(".so");
    if (!manager.openDLL(fname))
        return false;
    if (!manager.sendCallbackHandle())
    {
        qWarning("No setCallback !");
        return false;
    }
    WPTimbre *sample = (WPTimbre *)manager.newObject();
    if (!sample)
        return false;
    sample->set(para);
    manager.deleteObject(sample);
    return true;
}

WPTimbre *WPDLLTimbreManager::newTimbre()
{
    WPTimbre *t = (WPTimbre *)manager.newObject();
    t->set(para);
    timbres.push_back(t);
    //qDebug("TimbreManager %X newTimbre %X", (quint64)this, (quint64)t);
    return t;
}

void WPDLLTimbreManager::clearTimbres()
{
    std::vector<WPTimbre *>::iterator iter;
    for (iter = timbres.begin(); iter != timbres.end(); iter++)
        manager.deleteObject(*iter);
    timbres.clear();
}
