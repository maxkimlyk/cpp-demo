#include "demo-common.h"

#include <functional>

int func()
{
    return 0;
}

struct functor
{
    int operator() ()
    {
        return 1;
    }
};

struct SomeClass
{
    int value;
    int method()
    {
        return value;
    }
};

DEMO(function)
{
    // std::function is a general-purpose polymorphic function wrapper.
    // Instances of std::function can store, copy, and invoke any Callable target:
    // functions, lambdas, bind expressions, or other function objects,
    // as well as pointers to member functions and pointers to data members.

    std::function<int()> var;

    var = func;
    std::cout << "func result: " << var() << "\n";

    var = functor();
    std::cout << "functor result: " << var() << "\n";

    var = []() -> int { return 2; };
    std::cout << "lambda result: " << var() << "\n";

    var = std::bind(std::negate<int>(), -3);
    std::cout << "binding result: " << var() << "\n";

    SomeClass c = {5};
    std::function<int(SomeClass*)> func_member = &SomeClass::method;
    std::cout << "calling func member: " << func_member(&c) << "\n";
}


