#include <iostream>

template<typename ... T>
struct type_vector;

template<typename A0, typename B0>
struct equals
{
    static bool const value = false;
};

template<typename A>
struct equals<A, A>
{   
    static bool const value = true;
};

template<typename T1, typename T2>
struct is_equals;

template<typename A0, typename ... Args0, typename A1, typename ... Args1>
struct is_equals<type_vector<A0, Args0 ...>, type_vector<A1, Args1 ...>>
{
    static bool const value = is_equals<type_vector<Args0 ...>, type_vector<Args1 ...>>::value 
        && equals<A0, A1>::value;
};

template<typename A0, typename A1>
struct is_equals<type_vector<A0>, type_vector<A1>>
{
    static bool const value = equals<A0, A1>::value;
};

template<typename A, typename ... Args>
struct is_equals<type_vector<>, type_vector<A, Args ...>>
{
    static bool const value = false;
};

template<typename A1, typename ... Args1>
struct is_equals<type_vector<A1, Args1 ...>, type_vector<>>
{
    static bool const value = false;
};

int main()
{
    std::cout << std::boolalpha 
        << is_equals<type_vector<int, int, int>, type_vector<int, int, int>>::value 
        << std::endl;
    
    return 0;
}