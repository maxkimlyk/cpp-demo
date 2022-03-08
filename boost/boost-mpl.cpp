#include "demo-common.h"

#include <boost/mpl/int.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/multiplies.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/lambda.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>

// clang-format off

using namespace boost::mpl::placeholders;

// type representing int
using ten = boost::mpl::int_<10>;
using one = boost::mpl::int_<1>;

// type representing vector of values
using vector_2_3 = boost::mpl::vector_c<int, 2, 3>;

static_assert(ten::value == 10);

// metafuction classes (don't have template parameters but have template apply)
struct plus_1
{
    template <class X>
    struct apply
    {
        using type = boost::mpl::plus<X, boost::mpl::int_<1>>;
    };
};

template <class T>
struct add_pointer {
  using type = T*;
};

struct add_pointer_f {
  template <class T>
  struct apply : add_pointer<T> {};
};

// Evaluates F(F(X))
template <class F, class X>
struct twice :
  boost::mpl::apply<
    F,
    typename boost::mpl::apply<F, X>::type
  >
{};

static_assert(twice<plus_1, boost::mpl::int_<1>>::type::value == boost::mpl::int_<3>::value);
static_assert(std::is_same_v< twice<add_pointer_f, int>::type, int** >);

// placeholder expression: partial metafuction application
using double_f = boost::mpl::plus<_1, _1>;

// apply evaluates lambda expression (metafunction class or placeholder expression) on given arguments
// (is the same as invoking its nested apply metafunction)
using d1 = boost::mpl::apply<double_f, boost::mpl::int_<1>>::type;
static_assert(d1::value == 2);

// it can be also used with non-metafunction templates
using vector_of_int = boost::mpl::apply<std::vector<_1>, int>::type;
static_assert(std::is_same_v< vector_of_int, std::vector<int> >);

// metafunction composition
using m_plus_minus_f = boost::mpl::multiplies<
    boost::mpl::plus<_1,_2>, boost::mpl::minus<_1,_2>
  >;
static_assert(boost::mpl::apply<m_plus_minus_f, ten, one>::type::value == 11 * 9 );

// unnamed placeholder: n-th apprearance of it is transformed to _1, _2, etc.
using plus_f = boost::mpl::plus<_, _>; // is the same as plus<_1, _2>

RUN_DEMOS
