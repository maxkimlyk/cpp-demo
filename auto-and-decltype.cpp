#include "common.h"

int        foo1() { return 1; }
int&       foo2() { static int val = 2; return val; }
const int  foo3() { return 3; }
const int& foo4() { static int val = 4; return val; }

DEMO(auto)
{
    // auto
    //   T, const T, T&, const T& -> T

    auto var1 = foo1(); // int
    auto var2 = foo2(); // int
    auto var3 = foo3(); // int
    auto var4 = foo4(); // int

    // auto&
    //   T& -> T&
    //   const T& -> const T&

    //auto& var5 = foo1(); // error: initializing lvalue reference by rvalue
    auto& var6 = foo2(); // int&
    //auto& var7 = foo3(); // error: initializing lvalue reference by rvalue
    auto& var8 = foo4(); // const int&

    // auto&&
    //   T& -> T&
    //   const T& -> const T&
    //   T&&, const T&& -> T&&

    auto&& var9 = foo1();  // int&&
    auto&& var10 = foo2(); // int&
    auto&& var11 = foo3(); // int&&
    auto&& var12 = foo4(); // const int&
}

DEMO(decltype)
{
    // decltype(var) - exactly the same type, as var have

    int i1 = 1;
    int& i2 = i1;
    const int i3 = 3;
    const int& i4 = i1;

    decltype(i1) var1;        // int
    decltype(i2) var2 = var1; // int&
    decltype(i3) var3 = 0;    // const int
    decltype(i4) var4 = var1; // const int&

    // decltype(expr)
    //   lvalue T -> T& (const T&)
    //   rvalue T -> T
    //   xvalue (rvalue ref) -> T&&

    int i = 0;
    decltype(i) var5;         // int (like i1)
    decltype(i + 2) var6;     // int (rvalue -> T)
    decltype((i)) var7 = i;   // int& (lvalue -> T&)
    decltype(i = 4) var8 = i; // int& (lvalue -> T&)

    decltype(foo1()) var9;      // int (rvalue -> T)
    decltype(foo2()) var10 = i; // int& (lvalue ref -> T&)
    decltype(foo3()) var11 = 0; // int (rvalue -> const T)
    decltype(foo4()) var12 = i; // const int& (const lvalue ref -> T&)

    var11 = 3;

    // decltype(auto) var = expr; is equivalent to decltype(expr)

    decltype(auto) var13 = foo1(); // int 
    decltype(auto) var14 = foo2(); // int&
    decltype(auto) var15 = foo3(); // int
    decltype(auto) var16 = foo4(); // const int
}

// decltype(auto) function { ... }

decltype(auto) func() // equivalent to decltype(5 + 1)
{
    return 5 + 1;
}

RUN_DEMOS
