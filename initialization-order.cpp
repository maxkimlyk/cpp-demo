#include "common.h"

struct Base1
{
    int v;
    Base1(int v) : v(v)
    {
    }
};

struct Base2
{
    int v;
    Base2(int v) : v(v)
    {
    }
};

struct BadOrder : public Base1, public Base2
{
    int a;
    int b;

    BadOrder(int x) : b(x), a(b), Base2(a), Base1(Base2::v)
    {
    }
};

struct RightOrder : public Base1, public Base2
{
    int b;
    int a;

    RightOrder(int x) : Base1(x), Base2(Base1::v), b(Base2::v), a(b)
    {
    }
};

DEMO(initialization_order)
{
    // Initialization order is the same as the order that members declared in class,
    // but not the order that members are listed arter ":" in constructor!

    // First base objects are initialized (in order they are listed),
    // then members (in order they are listed).

    BadOrder bad_order(5);
    std::cout << "bad_order.a = " << bad_order.a << "\n";
    std::cout << "bad_order.b = " << bad_order.b << "\n";
    std::cout << "bad_order.Base1::v = " << bad_order.Base1::v << "\n";
    std::cout << "bad_order.Base2::v = " << bad_order.Base2::v << "\n";

    RightOrder right_order(5);
    std::cout << "right_order.a = " << right_order.a << "\n";
    std::cout << "right_order.b = " << right_order.b << "\n";
    std::cout << "right_order.Base1::v = " << right_order.Base1::v << "\n";
    std::cout << "right_order.Base2::v = " << right_order.Base2::v << "\n";
}

RUN_DEMOS
