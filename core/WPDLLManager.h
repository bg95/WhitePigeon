#ifndef WPDLLMANAGER_H
#define WPDLLMANAGER_H

#include <QString>

#ifdef Q_OS_WIN
    #include <windows.h>
#else
    #ifdef Q_OS_LINUX
        #include <dlfcn.h>
    #else
        #error "OS not supported"
    #endif
#endif

class WPDLLManager
{
public:
    #ifdef Q_OS_WIN
        typedef HINSTANCE HandleType;
    #else
        #ifdef Q_OS_LINUX
            typedef void *HandleType;
        #else
            #error "OS not supported"
        #endif
    #endif
    static HandleType openDLL(QString &name);
    static void *getFuncAddr(HandleType _handle, QString &func);
    static void closeDLL(HandleType _handle);

public:
    explicit WPDLLManager(QString &name);
    ~WPDLLManager();
    void *getFuncAddr(QString &func) const;

private:
    HandleType handle;

};

#endif // WPDLLMANAGER_H
