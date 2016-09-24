#include <iostream>

template <int M, int N>
struct A
{
    static void printType() { std::cout << "A<" << M << ", " << N << "> "; }
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

template <int... As>
struct Wrapper1
{
    template <int... Bs>
    static void print() { typePrinter<A<As, Bs>...>(); }; // Both will expand together!
};

int main(int argc, char* argv[])
{
    Wrapper1<1, 2, 3, 4, 5>::print<10, 20, 30, 40, 50>();
    std::cout << std::endl;
    return 0;
}
