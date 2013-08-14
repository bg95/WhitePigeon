#include "WPDLLManager.h"

WPDLLManager::WPDLLManager(QString &name)
{
    handle = openDLL(name);
}

WPDLLManager::~WPDLLManager()
{
    closeDLL(handle);
}

void *WPDLLManager::getFuncAddr(QString &func) const
{
    return getFuncAddr(handle, func);
}

WPDLLManager::HandleType WPDLLManager::openDLL(QString &name)
{
    QByteArray ba = name.toLocal8Bit();
    const char *str = ba.constData();
    #ifdef Q_OS_WIN
        return LoadLibrary(str);
    #else
        #ifdef Q_OS_LINUX
            return dlopen(str, RTLD_LAZY);
        #else
            #error "OS not supported"
        #endif
    #endif
}

WPDLLManager::HandleType WPDLLManager::getFuncAddr(HandleType _handle, QString &func)
{
    QByteArray ba = func.toLocal8Bit();
    const char *str = ba.constData();
    #ifdef Q_OS_WIN
        return GetProcAddress(_handle, str);
    #else
        #ifdef Q_OS_LINUX
            return dlsym(_handle, str);
        #else
            #error "OS not supported"
        #endif
    #endif
}

void WPDLLManager::closeDLL(HandleType _handle)
{
    #ifdef Q_OS_WIN
        FreeLibrary(_handle);
    #else
        #ifdef Q_OS_LINUX
            dlclose(_handle);
        #else
            #error "OS not supported"
        #endif
    #endif
}

