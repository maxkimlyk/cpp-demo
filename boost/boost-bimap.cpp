#include "demo-common.h"

#include <string>

#include <boost/bimap/bimap.hpp>

DEMO(bimap)
{
    using Bimap = boost::bimaps::bimap<int, std::string>;
    Bimap bimap;

    bimap.insert(Bimap::value_type(1, "one"));
    bimap.insert(Bimap::value_type(5, "five"));

    std::cout << "pair for 1 is " << bimap.left.find(1)->second << std::endl;
    std::cout << "pair for 'five' is " << bimap.right.find("five")->second << std::endl;
}


