#include "WPPropertyAndModifiers.h"
#include <QDebug>

std::string WPPropertyAndModifiers::ModifierPluginDir("/home/token/Documents/WhitePigeon/plugins/");

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
    manager.deleteObject(samplemodifier);
}

bool WPPropertyAndModifiers::setProperty(WPProperty &_prop)
{
    prop = _prop;

    std::istringstream iss(prop.getArg());
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
    qDebug() << " WPPropertyAndModifiers::setProperty name = " << name.data();
    iss >> para;
    fname.append(ModifierPluginDir);
    fname.append("lib");
    fname.append(name);
    fname.append(".so");
    qDebug() << "filename = \"" << fname.data() << '\"';
    if (!manager.openDLL(fname))
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
        modifiers.push_back(modi);

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
