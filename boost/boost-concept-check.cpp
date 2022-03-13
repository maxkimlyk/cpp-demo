#include <memory>

#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>

#include <demo-common.h>

template <class T>
void MakeCopy(const T& value)
{
    BOOST_CONCEPT_ASSERT((boost::CopyConstructible<T>));
    T other = value;
}

//clang-format off

template <class I>
BOOST_CONCEPT_REQUIRES(
    ((boost::RandomAccessIterator<I>))(
        (boost::LessThanComparable<typename boost::RandomAccessIterator<I>::value_type>)),
    (void)) // return type
sort(I begin, I end)
{
    std::sort(begin, end);
}

//clang-format on

template <class X>
class Incrementable : boost::Assignable<X>
{
    BOOST_CONCEPT_ASSERT((boost::DefaultConstructible<X>));

    BOOST_CONCEPT_USAGE(Incrementable)
    {
        X x = 0;
        ++x;
        x++;
    }
};

template <class Counter>
Counter Count()
{
    Counter c = 0;
    ++c;
    return c;
}

DEMO(usage)
{
    // Fails:
    // std::unique_ptr<int> ptr = nullptr;
    // MakeCopy(ptr);

    int i = 5;
    MakeCopy(i);

    int a[] = {1, 4, 3, 2, 6, 5};
    sort(a, a + 6);

    Count<int>();
}
