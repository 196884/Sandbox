#include <iostream>

template < typename T1, typename T2 >
bool isOneOf( T1&& a, T2&& b )
{
    return a == b;
}

template < typename T1, typename T2, typename ... Ts >
bool isOneOf( T1&& a, T2&& b, Ts&&... vs )
{
    return a == b || isOneOf( a, vs... );
}

int main( int argc, char * argv[] )
{
    bool c1 = isOneOf( 42, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 );
    bool c2 = isOneOf( 42, 1, 2, 3, 4, 5, 6, 7, 42, 9, 10 );
    std::cout << "c1: " << c1 << std::endl;
    std::cout << "c2: " << c2 << std::endl;
    return 0;
}
