#include <iostream>

#include "includes/big_integer.h"

using namespace std;

void test()
{
    storage s;
    vector<size_t> vec;
    cout << s.size() << ' ' << vec.size() << '\n';
    for (size_t i = 0; i != 10; ++i)
    {
        s.push_back(i);
        vec.push_back(i);
    }
    cout << s.size() << ' ' << vec.size() << '\n';
    while (s.size() > 0)
    {
        cout << s.back() << ' ' << vec.back() << ' ';
        s.pop_back();
        vec.pop_back();
    }
    cout << '\n' << s.size() << ' ' << vec.size() << '\n';
    for (size_t i = 0; i != 10; ++i)
    {
        s.push_back(i);
        vec.push_back(i);
    }
    cout << s.size() << ' ' << vec.size() << '\n';
}

int main()
{
    big_integer a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    big_integer b("100000000000000000000000000000000000000");
    big_integer c("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "00000000000000000000000000000000000000");

    a *= b;
    cout << a << '\n';
}
