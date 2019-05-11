#include "demo-common.h"
#include <utility>

class Class
{
public:
    Class(int v)
    {
        std::cout << "Class(int " << v << ")\n";
    }

    Class(const Class&)
    {
        std::cout << "Class(const Class&)\n";
    }

    Class(Class&&)
    {
        std::cout << "Class(Class&&)\n";
    }
};

DEMO(r_and_l_value_constructors)
{
    Class lvalue(1);
    Class c1 = lvalue;
    Class c2 = std::move(lvalue);
    Class c3(Class(3));
}

void func_by_copy(Class c)
{
    std::cout << " ^ func_by_copy\n";
}

template<class C>
void wrapper_by_copy(C&& c)
{
    std::cout << " ^ wrapper_by_copy\n";
    func_by_copy(std::forward<C>(c));
}

void func_by_ref(const Class& c)
{

}

template<class C>
void wrapper_by_ref(C&& c)
{
    std::cout << " ^ wrapper_by_ref\n";
    func_by_ref(std::forward<C>(c));
}

DEMO(perfect_forwarding)
{
    Class lvalue(1);

    // forward lvalue
    std::cout << "\nwrapper_by_copy(lvalue):\n";
    wrapper_by_copy(lvalue);

    // forward rvalue
    std::cout << "\nwrapper_by_copy(Class(3)):\n";
    wrapper_by_copy(Class(3));

    // forward lvalue
    std::cout << "\nwrapper_by_ref(lvalue):\n";
    wrapper_by_ref(lvalue);

    // forward rvalue
    std::cout << "\nwrapper_by_ref(Class(4)):\n";
    wrapper_by_ref(Class(4));
}

RUN_DEMOS
