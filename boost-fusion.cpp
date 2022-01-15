#include <boost/fusion/sequence.hpp>
#include <boost/fusion/algorithm.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/map.hpp>

#include "demo-common.h"

DEMO(vector)
{
    boost::fusion::vector<int, char, std::string> v(0, '0', "zero");
    std::cout << "int: " << boost::fusion::at_c<0>(v) << std::endl;
    std::cout << "char: " << boost::fusion::at_c<1>(v) << std::endl;
    std::cout << "string: " << boost::fusion::at_c<2>(v) << std::endl;
}

DEMO(for_each)
{
    const auto print_with_type = [](const auto& value) {
        std::cout << typeid(value).name() << ": " << value << std::endl;
    };

    boost::fusion::vector<int, char, std::string> v(0, '0', "zero");
    boost::fusion::for_each(v, print_with_type);

    using namespace boost::mpl::placeholders; // for _

    std::cout << "classes: " << std::endl;
    boost::fusion::for_each(boost::fusion::filter_if<boost::is_class<_>>(v), print_with_type);
}

namespace fields
{
struct key {};
struct name {};
struct value {};
} // namespace fields

DEMO(map)
{
    using my_obj = boost::fusion::map<boost::fusion::pair<fields::key, std::string>,
                                      boost::fusion::pair<fields::name, std::string>,
                                      boost::fusion::pair<fields::value, int>>;

    my_obj obj(boost::fusion::make_pair<fields::key>("0x0a01"),
               boost::fusion::make_pair<fields::name>("kitty"),
               boost::fusion::make_pair<fields::value>(23));

    auto serialize = [](const auto& map) {
        std::cout << "{";
        boost::fusion::for_each(map, [](const auto& pair) {
          using Pair = std::decay_t<decltype(pair)>;
          using First = typename Pair::first_type;
          std::cout << typeid(First).name() << ": " << pair.second << ", "; });
        std::cout << "}" << std::endl;
    };

    serialize(obj);
}

RUN_DEMOS
