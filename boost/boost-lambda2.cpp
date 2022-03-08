#include <boost/lambda2.hpp>

#include "demo-common.h"

int mod(int a, int b)
{
    return a % b;
}

DEMO(lambda2)
{
    using namespace boost::lambda2;

    const std::vector<int> c = {0, 1, 2, 3, 4, 5, 6, 7, 8};

    if (auto found = std::find_if(c.begin(), c.end(), _1 * 2 == 8); found != c.end())
    {
        std::cout << *found << std::endl;
    }

    if (auto found = std::find_if(c.begin(), c.end(), std::bind(mod, _1, 2) == 1); found != c.end())
    {
        std::cout << *found << std::endl;
    }
}

RUN_DEMOS
