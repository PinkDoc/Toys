#pragma once

template <typename T, typename G> struct is_same {
  enum { value = false };
};

// 特例
template <typename T> struct is_same<T, T> {
  enum { value = true };
};
