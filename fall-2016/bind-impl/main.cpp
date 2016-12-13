#include <iostream>

#include "bind.h"

using namespace std;

struct F
{
    int operator()(int a, int b)
    {
        return  a - b;
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
    auto b = bind(F(), (*f)(2, 3), (*g)(2, 3));
    auto c = bind(&f, (*g)(2, 3), (*f)(2, 3));
    cout << b() << endl;
    cout << F()(2, 3) << endl;
    cout << f((*g)(2, 3), f(2, 3)) << endl;
    cout << c() << endl;
    return 0;
}
