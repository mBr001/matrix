#ifndef _LIB_H_
#define _LIB_H_

#include <tuple>
#include <utility>

// Converts array into a tuple.
template<typename T, typename Array, size_t... I>
auto makeTupleImpl(const Array& a, std::index_sequence<I...>, T value)
{
    return std::make_tuple(a[I]..., value);
}

template<typename T, std::size_t N, typename Indices = std::make_index_sequence<N>>
auto makeTuple(const std::array<int, N>& arr, T value)
{
    return makeTupleImpl(arr, Indices{}, value);
}

#endif // _LIB_H