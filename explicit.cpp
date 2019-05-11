#include "demo-common.h"

struct C1
{
    C1(int) {}
    C1(int, int) {}
    operator bool() const {return true;};
};

struct C2
{
    explicit C2(int) {}
    explicit C2(int, int) {}
    explicit operator bool() const {return true;};
};

DEMO(explicit)
{
    C1 a1 = 1;       // C1::C1(int)
    C1 a2(2);        // C1::C1(int)
    C1 a3(3, 3);     // C1::C1(int, int)
    C1 a4 = {4, 4};  // C1::C1(int, int)
    C1 a5 = (C1)(5); // performs static_cast

    if (a1);         // C1::operator bool
    bool ba1 = a1;   // operator bool
    bool ba2 = static_cast<bool>(a1); // static cast

//  C2 b1 = 1;       // compilation error
    C2 b2(2);        // C2::C2(int)
    C2 b3(3, 3);     // C2::C2(int, int)
//  C2 b4 = {4, 4};  // compilation error
    C2 b5 = (C2)(5); // performs static_cast

    if (b2);         // calls explicitly C2::operator bool
//  bool bb1 = b2;   // compilation error: operator bool cannot be called implicitly
    bool bb2 = static_cast<bool>(b2); // static cast explicitly call C2::operator bool
}

RUN_DEMOS
