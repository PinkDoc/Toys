#pragma once

#include <cstddef>

// 17
template <typename T> constexpr std::size_t type_size = sizeof(T);

template <typename T> struct type_size_t {
  enum { value = sizeof(T) };
};

template <typename T, std::size_t s> struct is_same_size {
  enum { value = sizeof(T) == s };
};

template <int... vals>
struct int_container {
};

template <int... vals>
struct has_one
{
    enum
    {
        value = false
    };
};

template <int m,int... n>
struct has_one 
{
    enum
    {
        value = m == 1 ? true : has_one<n...>::value
    };
};
