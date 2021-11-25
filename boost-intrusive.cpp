#include "demo-common.h"

#include <iterator>

#include <boost/intrusive/list.hpp>
#include <boost/intrusive/unordered_set.hpp>

// Difference between intrusive and non-intrusive containers:
// - non-intrusive containers store copies of values
// - intrusive containers don't stores copies of values, but uses the values itself
// - intrusive containers require object to have special fields (like *next and *previous in case of linked list)

DEMO(list)
{
    // --- first way - inherit from base hook ---
    struct Foo : public boost::intrusive::list_base_hook<>
    {
        int value;
        Foo(int value) : value(value)
        {
        }
    };

    Foo obj_1(1);
    Foo obj_2(2);
    Foo obj_3(3);

    boost::intrusive::list<Foo> foo_list;

    foo_list.push_back(obj_1);
    foo_list.push_back(obj_2);
    foo_list.push_back(obj_3);
    foo_list.pop_back();

    std::cout << "Foo list: ";
    std::transform(foo_list.begin(), foo_list.end(), std::ostream_iterator<int>(std::cout, ","), [](const Foo& foo) { return foo.value; });
    std::cout << std::endl;

    // --- alternative way - have a member hook ---
    struct Bar
    {
        boost::intrusive::list_member_hook<> hook_;
        int value;

        Bar(int value) : value(value)
        {
        }
    };

    Bar bar_1(1);
    Bar bar_2(2);
    Bar bar_3(3);

    using MemberHookOption = boost::intrusive::member_hook<Bar, boost::intrusive::list_member_hook<>, &Bar::hook_>;
    boost::intrusive::list<Bar, MemberHookOption> bar_list;

    bar_list.push_back(bar_1);
    bar_list.push_back(bar_2);
    bar_list.insert(std::next(bar_list.begin()), bar_3);

    std::cout << "Bar list: ";
    std::transform(bar_list.begin(), bar_list.end(), std::ostream_iterator<int>(std::cout, ","), [](const Bar& bar) { return bar.value; });
    std::cout << std::endl;
}

struct Foo : public boost::intrusive::unordered_set_base_hook<>
{
    int value;

    Foo(int value) : value(value)
    {
    }

    friend bool operator==(const Foo& a, const Foo& b)
    {
        return a.value == b.value;
    }

    friend std::size_t hash_value(const Foo& value)
    {
        return std::size_t(value.value);
    }
};

DEMO(unordered_set)
{
    using unordered_set = boost::intrusive::unordered_set<Foo>;

    std::vector<Foo> values;
    for (int i = 0; i < 100; ++i)
    {
        if (i % 2 == 0)
        {
            values.push_back(i);
        }
    }

    // unordered set and map containers require supplementary memory
    // pool to be given (which is also called buckets)
    using bucket = unordered_set::bucket_type;
    std::vector<bucket> buckets;
    buckets.resize(20);

    unordered_set set(unordered_set::bucket_traits(buckets.data(), buckets.size()));

    for (auto& foo : values)
    {
        set.insert(foo);
    }

    const auto lookup = [&set](int value) {
        std::cout << value << " ";
        if (set.count(Foo(value)))
        {
            std::cout << "is in the set";
        }
        else
        {
            std::cout << "is not in the set";
        }
        std::cout << std::endl;
    };

    lookup(4);
    lookup(5);
}

RUN_DEMOS
