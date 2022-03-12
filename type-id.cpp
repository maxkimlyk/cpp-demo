#include "demo-common.h"

#include <typeinfo>

struct Base {};
struct Derived : Base {};

struct BasePoly { virtual void foo() {} };
struct DerivedPoly : BasePoly {};

DEMO(typeid)
{
    // typeid returns type_info&
    char var;
    const std::type_info& ti = typeid(var);

    // examples

    int var_int;
    const int var_const_int = 0;
    std::string var_string;
    double* var_p_double;

    std::cout << "var_int : " << typeid(var_int).name() << "\n";               // int
    std::cout << "var_const_int : " << typeid(var_const_int).name() << "\n";   // int
    std::cout << "var_string : " << typeid(var_string).name() << "\n";         // std::basic_string<char, std::char_traits<char>, std::allocator<char> >
    std::cout << "var_p_double : " << typeid(var_p_double).name() << "\n";     // double*

    Derived var_d;
    Base& var_b_ref = var_d;

    std::cout << "var_b_ref : " << typeid(var_b_ref).name() << "\n";           // Base

    DerivedPoly var_d_poly;
    BasePoly& var_b_poly_ref = var_d_poly;

    std::cout << "var_b_poly_ref : " << typeid(var_b_poly_ref).name() << "\n"; // DerivedPoly

    std::cout << "2 + 3.0f : " << typeid(2 + 3.0f).name() << "\n";             // float
}


