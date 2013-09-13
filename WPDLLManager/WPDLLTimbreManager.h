#ifndef WPDLLTIMBREMANAGER_H
#define WPDLLTIMBREMANAGER_H

#include <string>
#include "core/WPTimbre.h"
#include "WPDLLManager.h"

class WPDLLTimbreManager
{
public:
    static std::string TimbrePluginDir;

    WPDLLTimbreManager();
    ~WPDLLTimbreManager();
    bool setTimbre(std::string timbrenamepara);
    WPTimbre *newTimbre();
    void clearTimbres();

private:
    std::vector<WPTimbre *> timbres;
    WPDLLManager manager;
    std::string name, para, fname;

};

#endif // WPDLLTIMBREMANAGER_H
