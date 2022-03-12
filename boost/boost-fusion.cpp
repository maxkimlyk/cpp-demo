#include <boost/fusion/include/sequence.hpp>
#include <boost/fusion/include/algorithm.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/set.hpp>
#include <boost/fusion/include/std_tuple.hpp>

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

// clang-format off
namespace fields
{
struct key{};
struct name {};
struct value {};
} // namespace fields
// clang-format on

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
            std::cout << typeid(First).name() << ": " << pair.second << ", ";
        });
        std::cout << "}" << std::endl;
    };

    serialize(obj);
}

DEMO(cons)
{
    // lisp-style list (has front (car) and tail (cdr))

    using boost::fusion::cons;

    cons<int, cons<std::string>> list(10, cons<std::string>("ten"));
    std::cout << boost::fusion::at_c<0>(list) << ", " << boost::fusion::at_c<1>(list) << std::endl;
}

DEMO(set)
{
    boost::fusion::set<int, short, float> s(1, 2, 3.5);
    std::cout << "float: " << boost::fusion::at_key<float>(s) << std::endl;
    std::cout << "has int?: " << std::boolalpha << boost::fusion::has_key<int>(s) << std::endl;
}

DEMO(std_tuple_adapter)
{
    const auto print_with_type = [](const auto& value) {
        std::cout << typeid(value).name() << ": " << value << std::endl;
    };

    auto tpl = std::make_tuple(0, '0', "zero");
    boost::fusion::for_each(tpl, print_with_type);
}


