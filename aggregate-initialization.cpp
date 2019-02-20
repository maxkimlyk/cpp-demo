#include "common.h"

class ClassWithDefaultCtor
{
public:
    int x;
    ClassWithDefaultCtor(): x(10) {}
    ClassWithDefaultCtor(int i): x(i) {}
};

struct Struct
{
    int a;
    double b;
    ClassWithDefaultCtor c;
};

DEMO(aggregate_initialization)
{
    Struct st = {3}; // if no value for member initialization passed,
                     // member is initialized by default or 0
    std::cout << st.a << "\n";
    std::cout << st.b << "\n";
    std::cout << st.c.x << "\n";
}

RUN_DEMOS
