#include "common.h"

#include <boost/range/join.hpp>

#include <deque>
#include <iostream>
#include <list>
#include <vector>

DEMO(join)
{
    // boost::join allow to build a chain of ranges.

    const std::deque<int> deq = {0,1,2,3,4};
    const std::vector<int> vec = {5,6,7,8,9};
    const std::list<int> list = {10,11,12,13,14};

    const auto joined1 = boost::join(deq,vec);

    for(auto i : boost::join(joined1, list))
        std::cout << i << " ";

    std::cout << "\n";
}

RUN_DEMOS
