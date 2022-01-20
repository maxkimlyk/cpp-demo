#include <string>
#include <cassert>

#include "demo-common.h"

#include <boost/hana.hpp>

DEMO(tuple)
{
    // tuple of values
    auto values = boost::hana::make_tuple(1, 2.5f, 1.3);
    auto values_plus_one = boost::hana::transform(values, [](auto v) { return v + 1; });
    std::cout << "values_plus_one: ";
    boost::hana::for_each(values_plus_one, [](const auto& str) { std::cout << str << ", "; });
    std::cout << std::endl;

    // tuple of "types" (type wrappers actually)
    auto types = boost::hana::make_tuple(boost::hana::type_c<int>, boost::hana::type_c<char*>,
                                         boost::hana::type_c<const std::string&>);
    auto no_pointers = boost::hana::remove_if(
        types, [](auto type) { return boost::hana::traits::is_pointer(type); });

    static_assert(no_pointers == boost::hana::make_tuple(boost::hana::type_c<int>,
                                                         boost::hana::type_c<const std::string&>));
}

DEMO(traits_check)
{
    auto applicable_to_to_string =
        boost::hana::is_valid([](auto&& x) -> decltype((void)std::to_string(x)) {});
    static_assert(applicable_to_to_string(1.0f));

    struct my_struct
    {
    };
    static_assert(!applicable_to_to_string(my_struct{}));
}

RUN_DEMOS
