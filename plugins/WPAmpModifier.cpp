#include <cstdio>
#include "WPAmpModifier.h"

//void *(*WPAmpModifier::callback)(WPCallbackManager::CallbackFunc) = 0;

WPAmpModifier::WPAmpModifier()
{
}

WPAmpModifier::~WPAmpModifier()
{
}

bool WPAmpModifier::isAmpModifier()
{
    return true;
}

void WPAmpModifier::set(std::string para)
{
    para.push_back(' ');
    if (sscanf(para.data(), "%lf", &amp) == EOF)
    {
        amp = 1.0;
    }
    printf("WPAmpModifier %llX amp = %lf\n", (long long unsigned)this, this->amp);
    fflush(stdout);
}

std::vector<double> WPAmpModifier::modifyAmp(double time, std::vector<double> amp)
{
    printf("WPAmpModifier plugin modifies amp!\n");
    printf("WPAmpModifier %llX amp = %lf\n", (long long unsigned)this, this->amp);
    for (std::vector<double>::iterator iter = amp.begin(); iter != amp.end(); iter++)
    {
    	printf("from %lf to ", *iter);
        (*iter) *= this->amp;
        printf("%lf\n", *iter);
    }
    fflush(stdout);
    return amp;
}

extern "C"
{
    WPModifier *create()
    {
        return new WPAmpModifier();
    }
    void destroy(WPModifier *p)
    {
        printf("destroy called\n");
        fflush(stdout);
        if (p)
            delete p;
    }
    void setCallbackStatic(void *)
    {}
    void setCallbackMember(typeof WPCallbackManager::callMember)
    {}
}
