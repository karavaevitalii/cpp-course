#include <iostream>

#include "bind.h"

using namespace std;

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
    auto b = bind(&g, (*f)(2, 3), (*g)(2, 3));
    cout << b(2, 3) << endl;
    return 0;
}
