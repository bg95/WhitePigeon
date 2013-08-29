#include "WPCallbackManager.h"
#include "WPDLLManager.h"

WPDLLManager::WPDLLManager()
{
}

WPDLLManager::~WPDLLManager()
{
    closeDLL();
}

bool WPDLLManager::openDLL(QString &name)
{
    QByteArray ba = name.toLocal8Bit();
    const char *str = ba.constData();
    return openDLL(str);
}
bool WPDLLManager::openDLL(std::string &name)
{
    char str[name.size() + 1];
    memcpy(str, name.data(), name.size());
    str[name.size()] = 0;
    return openDLL(str);
}
bool WPDLLManager::openDLL(const char *str)
{
    if (handle)
        closeDLL();
    #ifdef Q_OS_WIN
        handle = LoadLibrary(str);
    #else
        #ifdef Q_OS_LINUX
            handle = dlopen(str, RTLD_LAZY);
        #else
            #error "OS not supported"
        #endif
    #endif
    if (handle)
    {
        create = (void *(*)())getFuncAddr("create");
        destroy = (void (*)(void *))getFuncAddr("destroy");
    }
    return handle != 0;
}

void *WPDLLManager::getFuncAddr(QString &name) const
{
    QByteArray ba = name.toLocal8Bit();
    const char *str = ba.constData();
    return getFuncAddr(str);
}
void *WPDLLManager::getFuncAddr(const char *str) const
{
    if (!handle)
        return 0;
    #ifdef Q_OS_WIN
        return GetProcAddress(handle, str);
    #else
        #ifdef Q_OS_LINUX
            return dlsym(handle, str);
        #else
            #error "OS not supported"
        #endif
    #endif
}

void *WPDLLManager::newObject() const
{
    return create ? create() : 0;
}

bool WPDLLManager::deleteObject(void *obj) const
{
    if (destroy)
    {
        destroy(obj);
        return true;
    }
    return false;
}

bool WPDLLManager::sendCallbackHandle() const
{
    void (*setCallback)(void *) = (void (*)(void *))getFuncAddr("setCallback");
    if (setCallback)
    {
        setCallback((void *)WPCallbackManager::call);
        return true;
    }
    return false;
}

void WPDLLManager::closeDLL()
{
    #ifdef Q_OS_WIN
        FreeLibrary(handle);
    #else
        #ifdef Q_OS_LINUX
            dlclose(handle);
        #else
            #error "OS not supported"
        #endif
    #endif
    handle = 0;
}
