#include "demo-common.h"
#include <utility>

/* === Reference collapsing rules (C++11) ===
 *   (T&)&   => T&
 *   (T&&)&  => T&
 *   (T&)&&  => T&
 *   (T&&)&& => T&&
 */

namespace mystd
{
template<class T>
constexpr T&& forward(std::remove_reference_t<T>&& arg) noexcept
{
    return static_cast<T&&>(arg);
}
}


template<class T>
void relay(T&& arg)
{
}

DEMO(universal_reference)
{
    // T&& variable initialized with rvalue => rvalue reference
    relay(5); // T = int&&,  T&& = (int&&)&& = int&&

    // T&& variable initialized with lvalue => lvalue reference
    int x = 0;
    relay(x); // T = int&, T&& = (int&)&& = int&
}


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
