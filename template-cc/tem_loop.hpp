#pragma once

// 给定一个无符号整数，获得他的二进制中1的数量

template <unsigned input>
constexpr unsigned ones_count = (input % 2) + ones_count<input / 2>;

template <>
constexpr unsigned ones_count<0> = 0;

// 处理数组

template <unsigned... input>
constexpr unsigned sum  = 0;

template <unsigned cur, unsigned... input>
constexpr unsigned sum<cur, input...> = cur + sum<input...>;






