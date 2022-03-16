#include <sstream>

#include <boost/pfr.hpp>

#include <demo-common.h>

struct Aggregate
{
    int i;
    char ch;
    double d;
    std::string str;
};

DEMO(pfr_functions)
{
    Aggregate a = {1, 't', 3.0, "four"};
    Aggregate b = {1, 't', 3.1, "four"};

    std::cout << "a = " << boost::pfr::io(a) << std::endl;
    std::cout << "a[0] = " << boost::pfr::get<0>(a) << std::endl;
    std::cout << "a < b? = " << std::boolalpha << boost::pfr::lt_fields(a, b) << std::endl;
}

namespace my
{
struct Tripple
{
    int i;
    char ch;
    double d;
};
BOOST_PFR_FUNCTIONS_FOR(Tripple);

} // namespace my

DEMO(pfr_functions_for)
{
    my::Tripple a = {1, 'a', 3.0};
    my::Tripple b = {1, 'a', 3.1};

    std::cout << "a = " << a << std::endl;
    std::cout << "a < b? = " << std::boolalpha << (a < b) << std::endl;

    my::Tripple c;
    std::istringstream ss("{10, b, 0.1}");
    ss >> c;
    std::cout << "c = " << c << std::endl;
}
