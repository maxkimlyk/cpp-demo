#include "demo-common.h"

#include <numeric>

#include <boost/hof.hpp>

struct sum_f
{
    template <class T, class U>
    T operator()(T x, U y) const
    {
        return x + y;
    }
};

// declares: inline const constexpr auto sum = sum_f{};
BOOST_HOF_STATIC_FUNCTION(sum) = sum_f();
BOOST_HOF_STATIC_LAMBDA_FUNCTION(sum2) = [](auto x, auto y) { return x + y; };

template <class T, class U>
auto sub(T a, U b)
{
    return a - b;
}

DEMO(lift)
{
    // HOST_HOF_LIFT wraps template function into functional object with template operator()

    std::vector<int> v = {1, 2, 3};
    std::accumulate(v.begin(), v.end(), 10, BOOST_HOF_LIFT(sub));
}

BOOST_HOF_STATIC_FUNCTION(sum_p) = boost::hof::pipable(sum_f());
BOOST_HOF_STATIC_FUNCTION(sum_i) = boost::hof::infix(sum_f());

DEMO(adaptors)
{
    std::cout << (1 | sum_p(2) | sum_p(5)) << std::endl;

    // clang-format off
    std::cout << (1 <sum_i> 2 <sum_i> 3) << std::endl;
    // clang-format on
}

RUN_DEMOS
