#include <iostream>

template <int N>
struct A
{
    static void printType() { std::cout << "A<" << N << "> "; }
};

template <typename T>
void typePrinter()
{
    T::printType();
}

// Note: the 'enable_if' is needed because of the type resolution problem
template <typename T, typename... Ts>
typename std::enable_if<(sizeof...(Ts) > 0), void>::type typePrinter()
{
    T::printType();
    typePrinter<Ts...>();
}

int main(int argc, char* argv[])
{
    typePrinter<A<2>, A<3>, A<5>, A<7>, A<11>>();
    std::cout << std::endl;
    return 0;
}
