#ifndef WPPROPERTYANDMODIFIERS_H
#define WPPROPERTYANDMODIFIERS_H

#include <sstream>
#include "WPDLLManager/WPDLLManager.h"
#include "WPScore/WPProperty.h"
#include "include/core/WPModifier.h"

class WPPropertyAndModifiers
{
public:
    static std::string ModifierPluginDir;

    WPPropertyAndModifiers();
    explicit WPPropertyAndModifiers(WPProperty &_prop);
    ~WPPropertyAndModifiers();
    bool setProperty(WPProperty &_prop);
    void setNumModifiers(int n);
    WPModifier *sampleModifier();
    WPModifier *modifier(int s);
    std::string getName();
    bool operator <(WPPropertyAndModifiers &b)
    {
        return prop < b.prop;
    }

private:
    void clearModifiers();
    WPProperty prop;
    WPDLLManager manager;
    std::vector<WPModifier *> modifiers;
    WPModifier *samplemodifier;
    std::string name, para, fname;

};

#endif // WPPROPERTYANDMODIFIERS_H
