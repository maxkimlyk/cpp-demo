#ifndef __COMMON_H__
#define __COMMON_H__

#include <functional>
#include <iostream>
#include <string>
#include <vector>

static std::vector<struct __demo_base__*> __demos__;

struct __demo_base__
{
    virtual void operator()() const = 0;
    std::string __name__;

    __demo_base__(const std::string&& name) : __name__(name)
    {
        __demos__.push_back(this);
    }

};

#define DEMO(name) \
    struct __##name##_demo__ : public __demo_base__    \
    {                                     \
        __##name##_demo__() : __demo_base__(#name) {}  \
                                          \
        void operator()() const override; \
    } __##name##_inst__;                  \
                                          \
    void __##name##_demo__::operator()() const

#define RUN_DEMOS \
    int main()                                                     \
    {                                                              \
        for (auto demo : __demos__)                                \
        {                                                          \
            std::cout << "===== " << demo->__name__ << " =====\n"; \
            (*demo)();                                             \
            std::cout << "\n";                                     \
        }                                                          \
        return 0;                                                  \
    }

#endif
