#include <any>
#include <string>
#include <cassert>
#include <type_traits>

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

struct default_tag
{
};

template <class T, class F>
auto case_(F f)
{
    return boost::hana::make_pair(boost::hana::type_c<T>, f);
}

template <class F>
auto default_(F f)
{
    return case_<default_tag>(f);
}

template <class DefaultF>
auto switch_apply_f(std::any& v, DefaultF default_f)
{
    return default_f();
}

template <class DefaultF, class Case, class... Rest>
auto switch_apply_f(std::any& v, DefaultF& default_f, Case& cs, Rest&... rest)
{
    using T = typename decltype(+boost::hana::first(cs))::type;
    if (v.type() == typeid(T))
    {
        return boost::hana::second(cs)(*std::any_cast<T>(&v));
    }
    else
    {
        return switch_apply_f(v, default_f, rest...);
    }
}

auto switch_(std::any& v)
{
    return [&v](const auto... cs) {
        auto cases = boost::hana::make_tuple(cs...);

        auto default_case = boost::hana::find_if(cases, [](const auto& c) {
            return boost::hana::first(c) == boost::hana::type_c<default_tag>;
        });

        static_assert(default_case != boost::hana::nothing, "no default_ statement");

        auto rest = boost::hana::filter(cases, [](const auto& c) {
            return boost::hana::first(c) != boost::hana::type_c<default_tag>;
        });

        return boost::hana::unpack(rest, [&](const auto&... rest) {
            const auto& default_f = boost::hana::second(*default_case);
            return switch_apply_f(v, default_f, rest...);
        });
    };
}

DEMO(use_case_switch)
{
    std::vector<std::any> anies = {std::any(5), std::any("hello")};

    for (std::any& a : anies)
    {
        switch_(a)(case_<int>([](int x) { std::cout << "int: " << x << std::endl; }),
                   default_([]() { std::cout << "other_type" << std::endl; }));
    }
}

DEMO(integral_constant)
{
    constexpr boost::hana::integral_constant<int, 1> one;
    constexpr boost::hana::integral_constant<int, 2> two;
    constexpr boost::hana::integral_constant<int, 3> three;

    static_assert(one + two == three);
}

DEMO(branching)
{
    const int one = boost::hana::if_(boost::hana::true_c, 1, "one");
    const char* const str_one = boost::hana::if_(boost::hana::false_c, 1, "one");

    const auto access = [](auto value) {
        const auto can_be_dereferenced = boost::hana::is_valid([](auto x) -> decltype(*x) {});
        if constexpr(can_be_dereferenced(value)) {
            return *value;
        } else {
            return value;
        }
    };

    int five = 5;

    access(5);
    access(&five);
}

RUN_DEMOS
