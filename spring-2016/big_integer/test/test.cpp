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
    cout << s.size() << ' ' << vec.size() << '\n';
    for (size_t i = 0; i != 10; ++i)
    {
        s.push_back(i);
        vec.push_back(i);
    }
}

int main()
{
    big_integer a = 23;

    cout << (a >> 2/*, 5*/);

    a >>= 2;
    cout << (a/*, 5*/);
}
