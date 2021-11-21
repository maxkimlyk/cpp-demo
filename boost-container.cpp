#include "demo-common.h"

#include <iterator>

#include <boost/container/stable_vector.hpp>
#include <boost/container/flat_set.hpp>

DEMO(stable_vector)
{
    // drop-in replacement for std::vector with following features
    // * iterators stay valid when you change stable_vector
    // * it doesn't do any copies/moves of elements on relocate
    // * has no elements contiguity in memory like std::vector

    struct non_copyable_non_movable
    {
        int payload;

        non_copyable_non_movable(int payload) : payload(payload)
        {
        }

        non_copyable_non_movable(const non_copyable_non_movable&) = delete;
        non_copyable_non_movable(non_copyable_non_movable&&) = delete;
    };

    boost::container::stable_vector<non_copyable_non_movable> st_vec;
    st_vec.emplace_back(1);
    st_vec.emplace_back(2);
    st_vec.emplace_back(3);
    st_vec.emplace_back(4);

    auto iter = st_vec.begin() + 2;
    std::cout << "third element: " << iter->payload << std::endl;

    std::cout << "deleting first 2 elements" << std::endl;
    st_vec.erase(st_vec.begin());
    st_vec.erase(st_vec.begin());

    std::cout << "iterator is still valid, rest of values: ";
    std::transform(iter, st_vec.end(), std::ostream_iterator<int>(std::cout, ", "), [](const auto& value) { return value.payload; });
    std::cout << std::endl;
}

DEMO(flat_set_map) {
  // flat_set, flat_map, flat_multiset, flat_multimap use vector
  // of sorted values instead of tree for storing data.
  // * due to spacial contiguity of data, we get
  //   - faster iterators
  //   - random access iterators
  //   - faster search operations
  //   - less memory consumption
  // * slower insertion and erasure (according to vector)

  boost::container::flat_set<int> set = {1, 5, 9, 2, 4};
  set.insert(3);
  set.insert(5);

  std::cout << "set: ";
  std::copy(set.begin(), set.end(), std::ostream_iterator<int>(std::cout, ", "));
}

RUN_DEMOS
