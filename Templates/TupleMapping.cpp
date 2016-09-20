#include <iostream>
#include <tuple>

template <typename T>
struct FooFn
{
    static
    inline
    void apply(T& t) { t.foo(); }
};

struct A
{
    void foo()
    {
        std::cout << "A::foo() called" << std::endl;
    }
};

struct B
{
    void foo()
    {
        std::cout << "B::foo() called" << std::endl;
    }
};

struct C
{
    void foo()
    {
        std::cout << "C::foo() called" << std::endl;
    }
};

template <std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I == sizeof...(Tp), void>::type
apply(std::tuple<Tp...>& t)
{}

template <std::size_t I = 0, typename... Tp>
inline typename std::enable_if<I < sizeof...(Tp), void>::type
apply(std::tuple<Tp...>& t)
{
    FooFn<typename std::tuple_element<I, std::tuple<Tp...>>::type>::apply(std::get<I>(t));
    apply<I + 1, Tp...>(t);
}

int main(int argc, char* argv[])
{
    A a;
    B b;
    C c;
    auto myTuple = std::make_tuple(a, c, b);
    apply(myTuple);
    return 0;
}
