#include <iostream>

#include "bind.h"

struct F
{
    int operator()() const
    {
        return  4;
    }
};

int f(int a, int b)
{
    return a + b;
}

int g(int a, int b)
{
    return a * b;
}

int main()
{
    auto a = [](int a, int b) {
        return g(a, b);
    };
    auto b = bind::bind(a, bind::bind(F()), bind::bind(F()));
    auto c = bind::bind(f, 4, 5);
    auto e = bind::bind(&g, bind::_1, bind::_2);
    std::cout << b() << '\n';
    std::cout << c() << '\n';
    std::cout << e(3, 4) << '\n';

    auto f = [](int& a){
        a += 5;
    };

    int x = 5;
    auto bb = bind::bind(f, x);
    bb();
    bb();
    std::cout << x << '\n';

    return 0;
}
