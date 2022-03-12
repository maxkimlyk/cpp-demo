#include "demo-common.h"

#include <boost/icl/interval_set.hpp>
#include <boost/icl/separate_interval_set.hpp>
#include <boost/icl/split_interval_set.hpp>
#include <boost/icl/interval_map.hpp>
#include <boost/icl/split_interval_map.hpp>

using IntInterval = boost::icl::interval<int>;

DEMO(interval_set)
{
    // interval_set joins overlapped and contiguous intervals

    boost::icl::interval_set<int> set;
    set.insert(90);
    set.insert(IntInterval::right_open(40, 50));

    std::cout << set << std::endl;

    set.insert(IntInterval::closed(50, 89));

    std::cout << set << std::endl;
}

DEMO(separate_interval_set)
{
    // interval_set joins overlapped intervals but doesn't join contiguous

    boost::icl::separate_interval_set<int> set;

    set.insert(90);
    set.insert(IntInterval::right_open(40, 50));

    std::cout << set << std::endl;

    set.insert(IntInterval::closed(50, 89));

    std::cout << set << std::endl;
}

DEMO(split_interval_set)
{
    // split_interval_set splits intervals into not-overlapped ones
    // (keeps border points)

    boost::icl::split_interval_set<int> set;

    set.insert(90);
    set.insert(IntInterval::right_open(40, 50));

    std::cout << set << std::endl;

    set.insert(IntInterval::open(0, 100));

    std::cout << set << std::endl;
}

DEMO(interval_map)
{
    // interval_map splits intervals if they overlap and
    // aggregates values of overlapped intervals via operator +=

    boost::icl::interval_map<int, int> map;

    map += std::make_pair(IntInterval::closed(0, 10), 1);
    map += std::make_pair(IntInterval::closed(20, 30), 1);

    std::cout << map << std::endl;

    map += std::make_pair(IntInterval::closed(5, 25), 1);

    std::cout << map << std::endl;
}

DEMO(split_interval_map)
{
    // split_interval_map doesn't join contigous intervals (if they have the same associated value) unlike interval_map

    boost::icl::interval_map<int, int> map;

    map += std::make_pair(IntInterval::right_open(0, 10), 1);
    map += std::make_pair(IntInterval::closed(10, 20), 1);

    std::cout << map << std::endl;

    boost::icl::split_interval_map<int, int> split_map;

    split_map += std::make_pair(IntInterval::right_open(0, 10), 1);
    split_map += std::make_pair(IntInterval::closed(10, 20), 1);

    std::cout << split_map << std::endl;
}


