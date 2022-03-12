#include "demo-common.h"

struct Base
{
    virtual ~Base() {};
};

struct A : public Base {};
struct B : public Base {};

DEMO(dynamic_cast_demo)
{
    // dynamic_cast safely converts pointers and references to classes up,
    // down, and sideways along the inheritance hierarchy.

    Base* p1 = new Base;
    Base* p2 = (Base*)(new A);
    Base* p3 = (Base*)(new B);

    B* p1_b = dynamic_cast<B*>(p1);
    if (p1_b == nullptr)
        std::cout << "p1_b is null\n";

    B* p2_b = dynamic_cast<B*>(p2);
    if (p2_b == nullptr)
        std::cout << "p2_b is null\n";

    B* p3_b = dynamic_cast<B*>(p3);
    if (p3_b == nullptr)
        std::cout << "p3_b is null\n";

    try
    {
        B& r1_b = dynamic_cast<B&>(*p1);
    }
    catch (std::bad_cast e)
    {
        std::cout << e.what() << "\n";
    }
}


