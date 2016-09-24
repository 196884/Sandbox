#include <iostream>

template <int N>
struct A
{
    static constexpr int value = N;
};

template <typename... Ts>
struct type_list
{};

template <typename A, typename B>
struct type_pair
{
    static void print() { std::cout << "(" << A::value << ", " << B::value << ")"; }
};

// Concatenation
template <typename ...Xs>
struct concat {};

template <typename ...Xs, typename ...Ys>
struct concat<type_list<Xs...>, type_list<Ys...>>
{
    using type = type_list<Xs..., Ys...>;
};

// Cross-product
template <typename X, typename Y>
struct cross_product {};

template <typename ...Ys>
struct cross_product<type_list<>, type_list<Ys...>>
{
    using type = type_list<>;
};

template <typename X, typename ...Xs, typename ...Ys>
struct cross_product<type_list<X, Xs...>, type_list<Ys...>>
{
    using type = typename concat<
        type_list<type_pair<X,Ys>...>,
        typename cross_product<type_list<Xs...>, type_list<Ys...>>::type
    >::type;
};

// Printer
template <typename X>
struct Printer {};

template <>
struct Printer<type_list<>>
{
    static void print() { std::cout << std::endl; }
};

template <typename X, typename ...Xs>
struct Printer<type_list<X, Xs...>>
{
    static void print()
    {
        X::print();
        std::cout << " ";
        Printer<type_list<Xs...>>::print();
    }
};

int main(int argc, char* argv[])
{
    Printer<cross_product<
        type_list<A<1>, A<2>, A<3>, A<4>, A<5>>,
        type_list<A<10>, A<20>, A<30>>
    >::type>::print();
    return 0;
}
