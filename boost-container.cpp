#include "demo-common.h"

#include <iterator>

#include <boost/container/stable_vector.hpp>
#include <boost/container/flat_set.hpp>
#include <boost/container/slist.hpp>
#include <boost/container/static_vector.hpp>
#include <boost/container/small_vector.hpp>

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

DEMO(flat_set_map)
{
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

DEMO(slist)
{
    // slist is a singly linked list. Slist is a bit different than std::forward_list.
    // * std::forward_list doesn't have size() method but allows
    //   merging different lists via splice after in constant time
    // * slist tracks its size

    boost::container::slist<int> list = {1, 2, 3, 4, 5};

    std::cout << "list.size(): " << list.size(); // O(1)
    std::cout << std::endl;

    boost::container::slist<int> another_list = {9, 9, 9};
    list.splice_after(list.begin(), another_list); // O(size(another_list))

    std::cout << "list: ";
    std::copy(list.begin(), list.end(), std::ostream_iterator<int>(std::cout, ", "));
}

DEMO(static_vector)
{
    // static_vector has fixed size like array
    // but doesn't require elements to be initialized when it is creating.

    struct wo_def_ctor
    {
        wo_def_ctor(int v) : value(v)
        {
        }

        int value;
    };

    boost::container::static_vector<wo_def_ctor, 10> static_v;
    static_v.emplace_back(1);
    static_v.emplace_back(2);
    static_v.emplace_back(3);
    static_v.pop_back();

    std::cout << "static_v: ";
    std::transform(static_v.begin(), static_v.end(), std::ostream_iterator<int>(std::cout, ", "), [](const auto obj) { return obj.value; });
}

template <class T>
class VerboseAllocator : public std::allocator<T>
{
public:
    T* allocate(std::size_t n)
    {
        std::cout << "alocating " << n * sizeof(T) << " bytes" << std::endl;
        return std::allocator<T>::allocate(n);
    }
};

DEMO(small_vector)
{
    // small_vector contains some pre-allocated space for a few elements
    // but can be increased unlike static_vector

    constexpr size_t kPreallocSize = 4;
    boost::container::small_vector<int, kPreallocSize, VerboseAllocator<int>> vec;

    for (size_t i = 0; i < 8; ++i)
    {
        vec.push_back(i);
        std::cout << "pushed " << i << std::endl;
    }

    // it can be passed as small_vector_base in order not to pass template parameter of preallocated size
    const auto print_func = [](const boost::container::small_vector_base<int, VerboseAllocator<int>>& param) {
        std::cout << "vector contains: ";
        std::copy(param.begin(), param.end(), std::ostream_iterator<int>(std::cout, ", "));
        std::cout << std::endl;
    };

    print_func(vec);
}

RUN_DEMOS
