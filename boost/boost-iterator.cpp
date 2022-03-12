#include "demo-common.h"

// --- Concepts ---
//
// Value access concepts:
//
//   Suppose X is an iterator type,  R is std::iterator_traits<X>::reference,
//   T is std::iterator_traits<X>::value_type, v is a constant object of type T,
//   M is type of member m of class T.
//
//   Readable Iterator:
//   1) X is assignable and copy constructible
//   2) iterator_traits<X>::value_type (or T) is any non-reference, non-cv-qualified type
//   3) *a is convertible to T. If a == b then *a is equivalent to *b
//   4) a->m returns M&, and is equivalent to (*a).m
//
//   Writable Iterator:
//   1) X is copy constructible
//   2) defined *a = o for each type o of associated set of value types
//
//   Swapable Iterator:
//   1) X is copy constructible
//   2) iter_swap(a, b) -> void  is defined
//
//   Lvalue Iterator:
//   1) *a -> T&
//   2) If X is Writable Iterator then  a == b <=> *a and *b is the same object.
//      If X is Readable Iterator then  a == b => *a and *b is the same object.
//
//
// Iterator traversal concepts:
//
//   Suppose X is an iterator type, a and b are constant objects of type X,
//   r and s are mutable objects of type X, T is std::iterator_traits<X>::value_type,
//   and v is a constant object of type T.
//
//   Incrementable Iterator:
//   1) X is assignable and copy constructible
//   2) ++r -> X&
//   3) &r == &++r
//   4) r++ is valid expression
//   5) *r++ is valid expression
//   6) iterator_traversal<X>::type is convertible to incrementable_traversal_tag
//
//   Single Pass Iterator:
//   1) X is Incrementable Iterator
//   2) X is equality comparable
//   3) ++r -> X&, besides if r is dereferenceable before, then r is dereferenceable or pass-to-end after
//   4) a == b  is convertible to bool
//   5) a != b  is convertible to bool
//   6) defined iterator_traits<X>::difference_type
//   7) iterator_traversal<X>::type is convertible to single_pass_iterator_tag
//
//   Forward Traversal Iterator:
//   1) X is Single pass Iterator
//   2) X is default constructible
//   3) X u; -> X&  (u may have singular value)
//   4) ++r -> X&
//   5) r == s and r is dereferenceable => ++r == ++s
//   6) iterator_traversal<X>::type is convertible to forward_traversal_tag
//
//   Bidirectional Traversal Iterator:
//   1) X is Forward Traversal Iterator
//   2) --r -> X&;
//        there exists s such that r == ++s  =>  s is dereferenceable
//        ++(--r) == r
//        --r == --s  =>  r == s
//        &r == &--r;
//   3) r-- returns convertible to const X&
//        and represents the following semantics:
//        {
//          X tmp = r;
//          --r;
//          return tmp;
//        }
//   4) iterator_traversal<X>::type is convertible to bidirectional_traversal_tag
//
//   Random Access Traversal Iterator:
//   Suppose Distance is iterator_traits<X>::dirrefence_type, n is an object of type const Distance.
//   1) X is Bidirectional Traversal Iterator
//   2) r += n -> X&
//        and represents the following semantics:
//        {
//          Distance m = n;
//          if (m > 0) {
//            while (m--)
//              ++r;
//          } else {
//            while (m++) {
//              --r;
//            }
//          }
//          return r;
//        }
//   3) a + n, n + a -> X
//        and represents the following semantics:
//        {
//          X tmp = a;
//          return tmp += n;
//        }
//   4) r -= n -> X&
//        and represents the following semantics:
//        { return r += -n; }
//   5) a - n -> X
//        and represents the following semantics:
//        {
//          X tmp = a;
//          return tmp -= n;
//        }
//   6) a < b -> bool , a > b -> bool
//   7) a >= b -> bool , a <= b -> bool
//   8) b - a -> Distance
//        and represents the following semantics:
//        { return a < b ? distance(a, b) : -distance(b,a); }
//   9) if a is Readable Iterator,  a[n] -> convertible to T
//      if a is Writable Iterator,  a[n] = v -> convertible to T
//   10) iterator_traversal<X>::type is convertible to random_access_traversal_tag
//

// --- Iterator Facade ---
#include <boost/iterator/iterator_facade.hpp>

#include <optional>

template <class SubIterator>
class cycle_iterator : public boost::iterator_facade<
                           /* Defived = */ cycle_iterator<SubIterator>,
                           /* Value = */ int,
                           /* CategoryOrTraversal = */ boost::random_access_traversal_tag,
                           /* Reference = */ int&,
                           /* Difference = */ ptrdiff_t>
{
private:
    using Base = boost::iterator_facade<cycle_iterator<SubIterator>, int,
                                        boost::random_access_traversal_tag, int&, ptrdiff_t>;

public:
    cycle_iterator() = default;
    cycle_iterator(SubIterator begin, SubIterator end, std::optional<SubIterator> pos = std::nullopt)
        : begin_(begin), end_(end), pos_(pos ? *pos : begin)
    {
    }

private:
    friend class boost::iterator_core_access;

    void increment()
    {
        ++pos_;
        if (pos_ == end_)
        {
            pos_ = begin_;
        }
    }

    void decrement()
    {
        if (pos_ == begin_)
        {
            pos_ == end_ - 1;
        }
        else
        {
            --pos_;
        }
    }

    void advance(typename Base::difference_type n)
    {
        const typename Base::difference_type len = std::distance(begin_, end_);
        if (len == 0)
        {
            return;
        }
        n = n % len;

        const typename Base::difference_type to_end = std::distance(pos_, end_);
        if (n < to_end)
        {
            pos_ += n;
        }
        else
        {
            pos_ = begin_ + (n - to_end);
        }
    }

    typename Base::difference_type distance_to(const cycle_iterator& other) const
    {
        return std::distance(pos_, other.pos_);
    }

    bool equal(const cycle_iterator& other) const
    {
        return pos_ == other.pos_;
    }

    typename Base::reference dereference() const
    {
        return *pos_;
    }

    SubIterator begin_;
    SubIterator end_;
    SubIterator pos_;
};

DEMO(iterator_facade)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto citer = cycle_iterator(v.begin(), v.end());

    std::cout << "cycle_iterator: ";
    for (int i = 0; i < 15; ++i, ++citer)
    {
        std::cout << *citer << ", ";
    }
    std::cout << std::endl;
}

// --- Iterator adaptor ---
#include <boost/iterator/iterator_adaptor.hpp>

template <class BaseIter>
class cycle_iterator_adaptor : public boost::iterator_adaptor<
                                   /* Derived = */ cycle_iterator_adaptor<BaseIter>,
                                   /* Base = */ BaseIter,
                                   /* Value = */ boost::use_default,
                                   /* Traversal = */ boost::use_default,
                                   /* Reference = */ boost::use_default,
                                   /* Dereference = */ boost::use_default>
{
private:
    using Base =
        boost::iterator_adaptor<cycle_iterator_adaptor<BaseIter>, BaseIter, boost::use_default,
                                boost::use_default, boost::use_default, boost::use_default>;

public:
    cycle_iterator_adaptor() = default;

    cycle_iterator_adaptor(BaseIter begin, BaseIter end, std::optional<BaseIter> pos = std::nullopt)
        : cycle_iterator_adaptor::iterator_adaptor_(pos ? *pos : begin), begin_(begin), end_(end)
    {
    }

private:
    friend class boost::iterator_core_access;

    void advance(typename Base::difference_type n)
    {
        const typename Base::difference_type len = std::distance(begin_, end_);
        if (len == 0)
        {
            return;
        }
        n = n % len;

        const typename Base::difference_type to_end = std::distance(this->base_reference(), end_);
        if (n < to_end)
        {
            this->base_reference() += n;
        }
        else
        {
            this->base_reference() = begin_ + (n - to_end);
        }
    }

    void increment()
    {
        ++this->base_reference(); // returns reference to base iterator
        if (this->base_reference() == end_)
        {
            this->base_reference() = begin_;
        }
    }

    void decrement()
    {
        if (this->base_reference() == begin_)
        {
            this->base_reference() = end_;
        }
        --this->base_reference();
    }

    BaseIter begin_;
    BaseIter end_;
};

DEMO(iterator_adaptor)
{
    const std::vector<int> v = {1, 2, 3, 4, 5};
    auto citer = cycle_iterator_adaptor(v.begin(), v.end());

    std::cout << "cycle_iterator_adaptor: ";
    for (int i = 0; i < 15; ++i, ++citer)
    {
        std::cout << *citer << ", ";
    }
    std::cout << std::endl;
}

// --- Specialized Adaptors ---
#include <boost/iterator/counting_iterator.hpp>

DEMO(counting_iterator)
{
    std::copy(boost::counting_iterator<int>(0), boost::counting_iterator<int>(10),
              std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;
}

#include <boost/iterator/filter_iterator.hpp>

DEMO(filter_iterator)
{
    std::vector<int> v = {0, -1, 2, -3, 4, -5, 6};

    const auto is_positive = [](int a) { return a > 0; };

    std::copy(boost::make_filter_iterator(is_positive, v.begin()),
              boost::make_filter_iterator(is_positive, v.end()),
              std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;
}

#include <boost/iterator/function_input_iterator.hpp>

DEMO(function_input_iterator)
{
    // allow incapsulating function object into iterator while tracking how many times function was invoked.
    const auto gen = []() { return rand(); };

    std::copy(boost::make_function_input_iterator(gen, 0),
              boost::make_function_input_iterator(gen, 6), std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;
}

#include <boost/function_output_iterator.hpp>

DEMO(function_output_iterator)
{
    // applies function to each assigned value
    const auto fun = [](const auto& something) { std::cout << something << std::endl; };

    std::vector<int> v = {1, 2, 3, 4};

    std::copy(v.begin(), v.end(), boost::make_function_output_iterator(fun));
}

#include <boost/iterator/indirect_iterator.hpp>
#include <memory>

DEMO(indirect_iterator)
{
    // applies extra operator* on dereference

    const std::vector v = {
        std::make_shared<int>(1),
        std::make_shared<int>(2),
        std::make_shared<int>(3),
    };

    std::copy(boost::make_indirect_iterator(v.begin()), boost::make_indirect_iterator(v.end()),
              std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;
}

#include <boost/iterator/permutation_iterator.hpp>

DEMO(permutation_iterator)
{
    // returns elements read from random access iterator according to given index range

    const std::vector<char> chars = {'a', 'b', 'c', 'd', 'e', 'f', 'g'};
    const std::vector<int> indices = {6, 1, 1, 3, 5};

    std::copy(boost::make_permutation_iterator(chars.begin(), indices.begin()),
              boost::make_permutation_iterator(chars.begin(), indices.end()),
              std::ostream_iterator<char>(std::cout, ", "));
    std::cout << std::endl;
}

#include <boost/iterator/reverse_iterator.hpp>

DEMO(reverse_iterator)
{
    std::vector<int> v = {1, 2, 3, 4, 5};

    std::copy(boost::make_reverse_iterator(v.end()), boost::make_reverse_iterator(v.begin()),
              std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;
}

#include <boost/shared_container_iterator.hpp>
#include <boost/shared_ptr.hpp>

DEMO(shared_container_iterator)
{
    // conbines shared_ptr with iterator to guarantee lifetime of the container

    const auto f = []() {
        auto v = boost::shared_ptr<std::vector<int>>(
            new std::vector<int>()); // works only with boost::shared_ptr, it sucks...
        v->push_back(1);
        v->push_back(2);
        v->push_back(3);
        return std::make_pair(boost::make_shared_container_iterator(v->begin(), v),
                              boost::make_shared_container_iterator(v->end(), v));
    };

    const auto [begin, end] = f();
    std::copy(begin, end, std::ostream_iterator<int>(std::cout, ","));
    std::cout << std::endl;
}

#include <boost/iterator/transform_iterator.hpp>

DEMO(transform_iterator)
{
    const std::vector<int> v = {1, 2, 3, 4, 5};

    const auto f = [](int i) { return i % 2; };

    std::copy(boost::make_transform_iterator(v.begin(), f),
              boost::make_transform_iterator(v.end(), f), std::ostream_iterator<int>(std::cout, ", "));
    std::cout << std::endl;
}

#include <boost/iterator/zip_iterator.hpp>
#include <string>

DEMO(zip_iterator)
{
    const std::vector<int> indices = {1, 2, 3, 4, 5};
    const std::vector<std::string> words = {"one", "two", "three", "four", "five"};

    // Ranges must have the same size! It is UB otherwise.

    // and it doesn't work with std::tuple
    const auto begin = boost::make_zip_iterator(boost::make_tuple(indices.begin(), words.begin()));
    const auto end = boost::make_zip_iterator(boost::make_tuple(indices.end(), words.end()));

    const auto print_pair = [](const auto& pair) {
        std::cout << pair.template get<0>() << ", " << pair.template get<1>() << std::endl;
    };

    std::copy(begin, end, boost::make_function_output_iterator(print_pair));
}


