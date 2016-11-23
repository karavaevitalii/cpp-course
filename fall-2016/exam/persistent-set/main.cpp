#include <iostream>

#include "persistent_set.h"

using namespace std;

int main()
{
    persistent_set s;
    for (int i = 0; i != 10; ++i)
        s.insert(i);

    for (auto it = s.begin(); it != s.end(); ++it)
    {
        cout << *it << ' ';
    }

    cout << '\n';

    for (int i = 0; i != 10; ++i)
    {
        cout << *s.find(i) << ' ';
        s.erase(s.find(i));
    }

    cout << '\n';

    for (int i = 0; i != 10; ++i)
        s.insert(2 * i);

    for (int i = 0; i != 10; ++i)
    {
        cout << *s.find(i * 2) << ' ';
        s.erase(s.find(i * 2));
    }
    return EXIT_SUCCESS;
}
