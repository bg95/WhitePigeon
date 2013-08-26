#include "WPDLLTimbreManager.h"

WPDLLTimbreManager::WPDLLTimbreManager()
{
}

WPDLLTimbreManager::~WPDLLTimbreManager()
{
    clearTimbres();
}

void WPDLLTimbreManager::setTimbre(std::string timbrenamepara)
{
    std::istringstream iss(timbrenamepara.getArg());
    char c;
    name.clear();
    para.clear();
    fname.clear();
    while (iss >> c && c != ' ')
    {
        name.push_back(c);
    }
    iss >> para;
    fname.append("lib");
    fname.append(name);
    fname.append(".so");
    if (!manager.openDLL(name))
        return false;
    if (!manager.sendCallbackHandle())
    {
        qWarning("No setCallback !");
        return false;
    }
    WPTimbre *sample = manager.newObject();
    if (!sample)
        return false;
    sample.set(para);
    manager.deleteObject(sample);
    return true;
}

WPTimbre *WPDLLTimbreManager::newTimbre()
{
    WPTimbre *t = manager.newObject();
    t->set(para);
    timbres.push_back(t);
    return t;
}

void WPDLLTimbreManager::clearTimbres()
{
    std::vector<WPTimbre *>::iterator iter;
    for (iter = timbres.begin(); iter != timbres.end(); iter++)
        manager.deleteObject(*iter);
    timbres.clear();
}
