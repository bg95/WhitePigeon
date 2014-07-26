#include "WPCallbackManager.h"
#include "WPDLLManager.h"

#include <cstdio>

WPDLLManager::WPDLLManager() :
    handle(0),
    create(0),
    destroy(0)
{
    objects.clear();
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
        std::wstringstream WStream;
        WStream << str;
        handle = LoadLibrary(WStream.str().data());
    #else
        #ifdef Q_OS_LINUX
            handle = dlopen(str, RTLD_NOW);
        #else
            #error "OS not supported"
        #endif
    #endif
    if (handle)
    {
        create = (void *(*)())getFuncAddr("create");
        destroy = (void (*)(void *))getFuncAddr("destroy");
    }

    printf("openDLL %s %X\n", str, (unsigned long long)handle);
    if (!handle)
        printf("openDLL error: %s\n", dlerror());
    fflush(stdout);

    return handle && create && destroy;
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
        return (void *)GetProcAddress(handle, str);
    #else
        #ifdef Q_OS_LINUX
            return dlsym(handle, str);
        #else
            #error "OS not supported"
        #endif
    #endif
}

void *WPDLLManager::newObject()
{
    if (create)
    {
        void *p = create();
        objects.insert(p);
        return p;
    }
    return 0;
}

bool WPDLLManager::deleteObject(void *obj)
{
    if (destroy)
    {
        std::set<void *>::iterator iter = objects.find(obj);
        if (iter != objects.end())
        {
            objects.erase(iter);
            destroy(obj);
            return true;
        }
    }
    return false;
}

bool WPDLLManager::clearObjects()
{
    if (!destroy)
        return false;
    while (!objects.empty())
    {
        deleteObject(*objects.begin());
    }
    return true;
}

typedef MembFuncPtr typeof_callMember(WPCallbackManager::CallbackFuncMember func);

bool WPDLLManager::sendCallbackHandle() const
{
    void (*setCallbackStatic)(void *) = (void (*)(void *))getFuncAddr("setCallbackStatic");
    if (!setCallbackStatic)
        return false;
    setCallbackStatic((void *)WPCallbackManager::callStatic);
    void (*setCallbackMember)(typeof_callMember) = (void (*)(typeof_callMember))getFuncAddr("setCallbackMember");
    if (!setCallbackMember)
        return false;
    setCallbackMember(WPCallbackManager::callMember);
    return true;
}

void WPDLLManager::closeDLL()
{
    if (!handle)
        return;
    clearObjects();
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
