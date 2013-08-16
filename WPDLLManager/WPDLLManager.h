#ifndef WPDLLMANAGER_H
#define WPDLLMANAGER_H

#include <QString>
#include "WPCallbackManager.h"

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
    /*
    static HandleType openDLL(QString &name);
    static void *getFuncAddr(HandleType _handle, QString &func);
    static void *getFuncAddr(HandleType _handle, const char *func);
    static void closeDLL(HandleType _handle);*/

public:
    WPDLLManager();
    //explicit WPDLLManager(QString &name);
    ~WPDLLManager();
    bool openDLL(QString &name);
    bool openDLL(const char *str);
    void *getFuncAddr(QString &name) const;
    void *getFuncAddr(const char *str) const;
    void *newObject() const; //assume there exists a constructor: void *create();
    bool deleteObject(void *obj) const;
    bool sendCallbackHandle() const;
    void closeDLL();

private:
    HandleType handle;
    void *(*create)();
    void (*destroy)(void *);

};

/*
 *To compile a DLL, use
 *g++ WPTuningFork.cpp -o libWPTuningFork.so -fPIC -shared -I /usr/include/qt5/QtMultimedia -I /usr/include/qt5/QtCore -I /usr/include/qt5 -g
 */

#endif // WPDLLMANAGER_H
