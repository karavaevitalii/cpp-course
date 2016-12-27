#include <iostream>

#include "bind.h"

struct F
{
    int operator()()
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

int sum(int a, int b)
{
    return a + b;
}

int main()
{
    auto a = [](int a, int b) {
        return g(a, b);
    };
    auto b = bind::bind(a, bind::bind(F()), bind::bind(F()));
    auto c = bind::bind(f, 4, 5);
    auto e = bind::bind(g, bind::placeholder::_1, bind::placeholder::_2);
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

    namespace bp = bind::placeholder;

    std::cout << (bind::bind(&sum, 5, -5))() << std::endl;
    std::cout << (bind::bind(&sum, bp::_1, 5))(-5) << std::endl;
    std::cout << (bind::bind(&sum, bp::_1, bp::_1))(0) << std::endl;
    std::cout << (bind::bind(&sum, bp::_2, bp::_2))(10, 0) << std::endl;
    std::cout << (bind::bind(&sum, bp::_2, bp::_1))(5, -5) << std::endl;
    std::cout << (bind::bind(&sum, bind::bind(&sum, bp::_2, 10), bp::_1))(5, -15) << std::endl;

    auto s = [](int a) {
        return a + 4;
    };

    auto d = bind::bind(s, 2);
    std::cout << d() << '\n';

    int xx = 4;
    bind::bind([](int& x){return x += 4;}, xx);
    std::cout << xx << '\n';

    return 0;
}
