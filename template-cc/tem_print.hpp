#pragma once

#include <iostream>

template <typename... Args> 
void print(Args... args)
{
}

template <typename T, typename... Args>
void print(T t, Args... args)
{
    std::cout << t;
    print(args...);
}

template <typename... Args> void println(Args... args) {
}

template <typename T, typename... Args> void println(T t, Args... args) {
  std::cout << t;
  print(args...);
}
