#pragma once

#include <cstddef>

template <typename T, typename... Args> struct template_len {
  enum { value = template_len<Args...>::value + 1 };
};

// 特例
template <typename T> struct template_len<T> {
  enum { value = 1 };
};
