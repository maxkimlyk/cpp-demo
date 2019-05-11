#include "demo-common.h"

// Inherit from each of passed lambdas.
template<class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

// Allow deduce template parameters of class from constructor.
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main()
 {
    const auto lambda = overloaded {
        [](int i)   { std::cout << "int: " << i << "\n"; },
        [](char ch) { std::cout << "char: " << ch << "\n"; }
    };

    lambda(1);
    lambda('a');

    return 0;
}
