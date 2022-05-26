#pragma once

#include <iostream>

template <typename S>
class Base {
public:
    void func()
    {
        S *d = (S *)(this);
        d->imple();
    }
};

class Son : public Base<Son> {
public:
    void imple()
    {
        std::cout << "Son::imple()" << std::endl;
    }
};

