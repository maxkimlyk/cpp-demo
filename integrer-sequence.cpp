#include "common.h"
#include <array>

// This is integer_sequence structure. Structure is empty,
// real sequence is in template parameters.
template <class T, T...>
struct integer_sequence {};

// To generate such sequence we need a specific generator.

template <class T, size_t N, T... Seq>
struct make_integer_sequence_impl : make_integer_sequence_impl<T, N-1, N, Seq...> {};

template <class T, T... Seq>
struct make_integer_sequence_impl<T, 0, Seq...>
{
    using type = integer_sequence<T, 0, Seq...>;
};

template <class T, size_t N>
using make_integer_sequence = typename make_integer_sequence_impl<T, N-1>::type;

// Usage example.

template <class T, class Func, size_t... IndexSeq>
std::array<T, sizeof...(IndexSeq)> construct_array_impl(Func func, integer_sequence<size_t, IndexSeq...>)
//                                                                             ^
//                             fictive parameter, just to pass `size_t... IndexSeq` in template parameters
{
    return {func(IndexSeq)...}; // using initializer list
}

template <class T, size_t Size, class Func>
std::array<T, Size> construct_array(Func generate_func)
{
    return construct_array_impl<T>(generate_func, make_integer_sequence<size_t, Size>());
}

DEMO(integer_sequence)
{
    const size_t size = 10;
    auto arr = construct_array<float, 10>([] (size_t idx) -> float { return (float)(idx) * 1.1f; });

    std::cout << "arr: ";
    for (auto a : arr)
        std::cout << a << " ";
}

RUN_DEMOS
