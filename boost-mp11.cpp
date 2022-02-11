#include <boost/mp11.hpp>
#include <boost/core/demangle.hpp>

#include <functional>

#include "demo-common.h"

template <class Fst, class Snd, class Trd>
class your_type
{
};

// ==== DICTIONARY ====
// clang-format off

// List is any template class whose parameters are types
// For example:
using list1 = boost::mp11::mp_list<int, char*>;
using list2 = boost::mp11::mp_list<>;
using list3 = std::tuple<int, float, char>;
using list4 = your_type<int, short, float>;

// Metafunction is class template or template alias whose parameters are types
// For example:
template<class...> using F1 = void;
template<class T> using F2 = T*;
template<class... T> using F3 = std::integral_constant<size_t, sizeof...(T)>;

// Quoted metafunction is a class with public metafunction member called fn
struct Q1 { template<class... T> using fn = F1<T...>; };
struct Q2 { template<class T> using fn = std::integral_constant<size_t, sizeof(T) + 1>; };

// Integral constant type is a class with public constant member value
struct IC { static int constexpr value = 148; };

// clang-format on

DEMO(for_each)
{
    // for_each calls F(t()) for each type in the given list

    using types = std::tuple<int, short, char*, std::string, std::function<int(int)>>;
    boost::mp11::mp_for_each<types>([](auto&& value) {
        char const* name = typeid(value).name();
        std::cout << boost::core::demangle(name) << ": " << sizeof(value) << " bytes" << std::endl;
    });
}

RUN_DEMOS
