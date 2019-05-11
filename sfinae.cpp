#include "demo-common.h"

template<class T>
struct can_be_dereferenced
{
    template<class U>
    static constexpr decltype(*std::declval<U>(), true) detect(int)
    {
        return true;
    }

    template<class>
    static constexpr bool detect(...)
    {
        return false;
    }

    static constexpr bool value = detect<T>(0);
};

DEMO(can_be_dereferenced)
{
    if constexpr (can_be_dereferenced<int*>::value)
        std::cout << "int* can be dereferenced.\n";
    else
        std::cout << "int* cannot be dereferenced.\n";

    if constexpr (can_be_dereferenced<int>::value)
        std::cout << "int can be dereferenced.\n";
    else
        std::cout << "int cannot be dereferenced.\n";
}

RUN_DEMOS
