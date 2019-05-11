#include "demo-common.h"

#include <optional>

std::optional<int> func(bool b)
{
    return b ? std::optional<int>(10) : std::nullopt;
}

DEMO(optional)
{
    auto var1 = func(false);
    auto var2 = func(true);

    if (var1) // operator bool overloaded
        std::cout << "var1 has value "
                  << *var1 // * returns containing object. Behavior is undefined when no object contained.
                  << "\n";

    if (var2.has_value())
        std::cout << "var2 has value " << var2.value() << "\n";

    std::cout << "var1 has value " << var1.value_or(-1) << " (-1 if empty)\n";
}

RUN_DEMOS
