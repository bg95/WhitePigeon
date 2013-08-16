#include "WPDLLTimbre.h"

WPDLLTimbre::WPDLLTimbre() :
    timbre(0),
    manager(0)
{
}

WPDLLTimbre::~WPDLLTimbre()
{
    if (manager)
    {
        if (timbre)
            manager->deleteObject(timbre);
        delete manager;
    }
    //manager = 0;
}

bool WPDLLTimbre::loadDLL(QString name)
{
    if (manager)
        delete manager;
    //manager = 0;
    manager = new WPDLLManager();
    if (!manager->openDLL(name))
        return false;
    if (!manager->sendCallbackHandle())
    {
        qWarning("No setCallback !");
        return false;
    }
    timbre = (WPTimbre *)manager->newObject();
    manager->deleteObject(timbre); //destructor not called, destructor of WPTimbre called
    timbre = (WPTimbre *)manager->newObject();
    if (!timbre)
        return false;

    //test constructor
    //constructor = (void (WPTimbre::*)())manager->getFuncAddr("_ZN12WPTuningForkC1Ev");
    //qDebug("test constructor %X", (quint64)constructor);

    return true;
}

WPWave *WPDLLTimbre::synthesize(double dur, double *amp, double *freq) const
{
    if (!timbre)
        return 0;
    return timbre->synthesize(dur, amp, freq);
}
