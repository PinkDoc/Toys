#pragma once

// function : y = f(x)
constexpr int func(int x) 
{
    // ok
    return x * x;
}

/*
// bad
static int y = 0;
constexpr int func(int x)
{
    return x  * (y + 1);
}
*/


template <bool check>
auto check_way() 
{
    if constexpr (check) {
        return (int)1;
    } else {
        return (double)1;
    }
}