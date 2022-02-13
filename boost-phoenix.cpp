#include <boost/phoenix.hpp>
#include <boost/phoenix/core/argument.hpp>

#include "demo-common.h"

using namespace boost::phoenix::arg_names;

DEMO(phoenix)
{
    auto v1 = boost::phoenix::val(1488);   // nullary function that returns 1488
    auto v2 = boost::phoenix::val("abcd"); // nullary function that returns const char*

    int i = 1;
    auto r1 = boost::phoenix::ref(i); // nullary function that returns int&

    std::cout << v1() << std::endl;
    std::cout << v2() << std::endl;
    std::cout << r1() << std::endl;

    std::vector<int> c = {0, 1, 2, 3, 4};
    auto found = std::find_if(c.begin(), c.end(), arg1 % 2 == 1);
    if (found != c.end())
    {
        std::cout << "found: " << *found << std::endl;
    }

    std::for_each(c.begin(), c.end(), boost::phoenix::if_(arg1 > 0)[std::cout << arg1 << " "]);
}

struct is_odd_impl
{
    using result_type = bool;

    template <typename Arg>
    bool operator()(Arg arg1) const
    {
        return arg1 % 2 == 1;
    }
};
boost::phoenix::function<is_odd_impl> is_odd;

DEMO(function)
{
    std::vector<int> c = {0, 1, 2, 3, 4};
    auto found = std::find_if(c.begin(), c.end(), is_odd(arg1));
    if (found != c.end())
    {
        std::cout << *found << std::endl;
    }
}

RUN_DEMOS
