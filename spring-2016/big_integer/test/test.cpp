#include <iostream>

#include "includes/big_integer.h"

using namespace std;


int main()
{
    big_integer a = 23;

    cout << ((a << 5) == (23 * 32));

    a <<= 5;
    cout << (a == (23 * 32));
}
