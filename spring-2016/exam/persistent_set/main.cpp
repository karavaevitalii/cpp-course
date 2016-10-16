#include <iostream>
#include <set>
#include "persistent_set.h"

using namespace std;

int main()
{
    persistent_set s;
    for (int i = 0; i != 10; ++i)
        s.insert(i);
    for (persistent_set::iterator it = s.begin(); it != s.end(); ++it)
        cout << *it << '\n';
}