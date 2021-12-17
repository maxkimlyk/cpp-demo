#include "demo-common.h"

#include <boost/range/join.hpp>
#include <boost/range/adaptor/adjacent_filtered.hpp>
#include <boost/range/adaptor/copied.hpp>
#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/indexed.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/indirected.hpp>
#include <boost/range/adaptor/map.hpp>
// #include <boost/range/adaptor/ref_unwrapped.hpp>
#include <boost/range/adaptor/replaced.hpp>
#include <boost/range/adaptor/replaced_if.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <boost/range/adaptor/sliced.hpp>
#include <boost/range/adaptor/strided.hpp>
#include <boost/range/adaptor/tokenized.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/adaptor/uniqued.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/range/algorithm_ext/erase.hpp>
#include <boost/range/any_range.hpp>
#include <boost/range/counting_range.hpp>
#include <boost/range/istream_range.hpp>
#include <boost/range/irange.hpp>

#include <deque>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

struct print_to_cout_adaptor
{
    template <class Rng>

    friend void operator|(const Rng& range, print_to_cout_adaptor)
    {
        using ElemType = std::decay_t<decltype(*range.begin())>;
        std::copy(range.begin(), range.end(), std::ostream_iterator<ElemType>(std::cout, ","));
    }
};

print_to_cout_adaptor print_to_cout;

DEMO(adjacent_filtered)
{
    // for each pair [x, y] of adjacent elements puts x to output if pred(x, y) satisfied

    const std::vector<int> vec = {1, 1, 2, 2, 2, 3, 2, 5, 5};
    const auto same_oddity = [](int lhs, int rhs) { return lhs != rhs; };

    vec | boost::adaptors::adjacent_filtered(same_oddity) | print_to_cout;
}

DEMO(copied)
{
    // puts sliced copy of range [n, m) ( m must be < distance(rng) )

    const std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vec | boost::adaptors::copied(2, 5) | print_to_cout;
}

DEMO(filtered)
{
    // puts elements for which prediate is true to output range

    const std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vec | boost::adaptors::filtered([](int value) { return value % 2 == 0; }) | print_to_cout;
}

DEMO(indexed)
{
    // puts index_value = (value, index) to output range

    const std::vector<char> vec = {'a', 'b', 'c', 'd', 'e'};

    vec | boost::adaptors::indexed(1) | boost::adaptors::transformed([](const auto& index_value) {
        std::ostringstream os;
        os << index_value.index() << ":" << index_value.value();
        return os.str();
    }) | print_to_cout;
}

DEMO(indirected)
{
    // maps range of pointers to range of pointed values

    const std::vector<std::shared_ptr<int>> pointers = {
        std::make_shared<int>(1), std::make_shared<int>(2), std::make_shared<int>(3), std::make_shared<int>(4), std::make_shared<int>(5),
    };

    pointers | boost::adaptors::indirected | print_to_cout;
}

DEMO(map)
{
    // map_keys puts x.first into output range
    // map_values puts y.second into output range

    const std::map<int, std::string> map = {
        {1, "one"},
        {2, "two"},
        {3, "three"},
    };

    std::cout << "keys: ";
    map | boost::adaptors::map_keys | print_to_cout;
    std::cout << std::endl;

    std::cout << "values: ";
    map | boost::adaptors::map_values | print_to_cout;
}

// DEMO(ref_unwrapped)
// {
//     // puts x.get() into output range

//     struct Data
//     {
//         int value;
//     };

//     Data a{0};
//     Data b{0};
//     Data c{0};

//     std::vector<std::reference_wrapper<Data>> references{a, b, c};

//     references | boost::adaptros::ref_unwrapped | print_to_cout;
// }

DEMO(replaced)
{
    // replaced replaced old value with new value

    const std::vector<int> vec = {1, 2, 3, 3, 5, 3, 4};

    std::cout << "3->0: ";
    vec | boost::adaptors::replaced(3, 0) | print_to_cout;
    std::cout << std::endl;

    std::cout << "odd->0: ";
    vec | boost::adaptors::replaced_if([](const int v) { return v % 2 == 1; }, 0) | print_to_cout;
}

DEMO(reversed)
{
    // reverses bidirectional range

    const std::vector<int> vec = {1, 2, 3, 4, 5};

    vec | boost::adaptors::reversed | print_to_cout;
}

DEMO(sliced)
{
    // slices range elements to [n, m) ( m must be < distance(rng) )

    const std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vec | boost::adaptors::sliced(2, 5) | print_to_cout;
}

DEMO(strided)
{
    // puts each n-th value to output range

    const std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vec | boost::adaptors::strided(3) | print_to_cout;
}

DEMO(tokenized)
{
    // splits string into tokens defined by boost::regex

    const std::string text = "  /a1/  /b2/ /c3/ aa bb /dc/ d4 5";

    const auto regex = boost::regex("/(\\w+)/");
    const int subgoup = 1;
    text | boost::adaptors::tokenized(regex, subgoup) | print_to_cout;
}

DEMO(transformed) {
    const std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    vec | boost::adaptors::transformed([](const int v){ return v * 2;}) | print_to_cout;
}

DEMO(uniqued) {
    // removes elements that have the same previous

    const std::vector<int> vec = {1, 1, 1, 2, 2, 2, 1, 1, 1};
    vec | boost::adaptors::uniqued | print_to_cout;
}

DEMO(join)
{
    // boost::join allow to build a chain of ranges.

    const std::deque<int> deq = {0, 1, 2, 3, 4};
    const std::vector<int> vec = {5, 6, 7, 8, 9};

    const auto joined1 = boost::join(deq, vec);

    joined1 | print_to_cout;
}

DEMO(algotirhm_unique) {
    const std::vector<int> original = {1, 1, 5, 5, 5, 4, 3};

    std::vector<int> vec = original;
    const auto unique_range = boost::unique(vec);

    std::cout << "unique_range: ";
    unique_range | print_to_cout;
    std::cout << std::endl;

    std::cout << "what actually vec holds: ";
    vec | print_to_cout;
    std::cout << std::endl;

    vec = original;
    auto iter = boost::unique<boost::return_found>(vec);
    std::cout << "by found iter: ";
    std::copy(vec.begin(), iter, std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;

    vec = original;
    const auto begin_next_range = boost::unique<boost::return_begin_next>(vec);
    std::cout << "begin-next: ";
    begin_next_range | print_to_cout;
    std::cout << std::endl;

    vec = original;
    const auto found_end_range = boost::unique<boost::return_found_end>(vec);
    std::cout << "found-end: ";
    found_end_range | print_to_cout;
    std::cout << std::endl;

    // the return policy may be helpful in cases like this: delete duplicates
    boost::erase(vec, boost::unique<boost::return_found_end>(boost::sort(vec)));
}

DEMO(any_range) {
    // any_range car represent vectors, lists or other range objects

    using int_range = boost::any_range<int, boost::bidirectional_traversal_tag>;

    const std::vector<int> vec = {1, 2, 3, 4};
    const std::list<int> list = {5, 6, 7, 8};

    int_range rng = vec;
    rng | print_to_cout;

    rng = list;
    rng | print_to_cout;
}

DEMO(counting_range) {
    // counting_range generates incrementing values
    auto rng = boost::counting_range(1, 5);
    rng | print_to_cout;
}

DEMO(istream_range) {
    // istream_range wraps istream_iterator

    const std::string text = "1 88 sts text word  d 87 ";
    std::istringstream stream(text);
    auto rng = boost::istream_range<int>(stream);
    rng | print_to_cout;
}

DEMO(irange) {
    // irange generates integer range

    const size_t first = 1;
    const size_t last = 10;
    const size_t step = 2;

    auto rng = boost::irange(first, last, step);
    rng | print_to_cout;
}

RUN_DEMOS
