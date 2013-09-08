#include "WPPropertyAndModifiers.h"

WPPropertyAndModifiers::WPPropertyAndModifiers()
{
}

WPPropertyAndModifiers::WPPropertyAndModifiers(WPProperty &_prop)
{
    setProperty(_prop);
}

WPPropertyAndModifiers::~WPPropertyAndModifiers()
{
    clearModifiers();
}

bool WPPropertyAndModifiers::setProperty(WPProperty &_prop)
{
    prop = _prop;

    std::istringstream iss(prop.getArg());
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
    samplemodifier = (WPModifier *)manager.newObject();
    if (!samplemodifier)
        return false;
    samplemodifier->set(para);
    return true;
}

void WPPropertyAndModifiers::setNumModifiers(int n)
{
    WPModifier *modi;
    clearModifiers();
    while (n--)
    {
        modi = (WPModifier *)manager.newObject();
        modi->set(para);
        modifiers.push_back((WPModifier *)manager.newObject());

    }
}

WPModifier *WPPropertyAndModifiers::sampleModifier()
{
    return samplemodifier;
}

std::string WPPropertyAndModifiers::getName()
{
    return name;
}

void WPPropertyAndModifiers::clearModifiers()
{
    std::vector<WPModifier *>::iterator iter;
    for (iter = modifiers.begin(); iter != modifiers.end(); iter++)
        manager.deleteObject(*iter);
    modifiers.clear();
}
