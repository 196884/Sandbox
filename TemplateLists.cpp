#include <iostream>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>

template <size_t N, size_t K>
struct IsPrime_Aux
{
    static const bool Result = ( N % K ) && IsPrime_Aux<N, K-1>::Result;
};

template <size_t N>
struct IsPrime_Aux<N, 1>
{
    static const bool Result = true;
};

template <size_t N>
struct IsPrime
{
    static const bool Result = IsPrime_Aux<N, N-1>::Result;
};

template <>
struct IsPrime<1>
{
    static const bool Result = false;
};

template <>
struct IsPrime<0>
{
    static const bool Result = false;
};

template <size_t N>
struct ListPrimes
{
    static void Exec()
    {
        ListPrimes<N-1>::Exec();
        if( IsPrime<N>::Result )
            std::cout << N << " ";
    }
};

template <>
struct ListPrimes<0>
{
    static void Exec() {}
};

// LISTS //

struct Empty
{
    static void print() { std::cout << "EMPTY"; }
};

template <typename HEAD, typename TAIL>
struct Cons
{
    typedef HEAD Head;
    typedef TAIL Tail;
    static void print() { HEAD::print(); std::cout << ", "; TAIL::print(); }
};

template <size_t N>
struct UInt
{
    typedef size_t Type;
    static const size_t Value = N;
    static void print() { std::cout << N; }
};

template <typename DONE, typename TODO>
struct Reverse_Aux
{
    using Aux = typename Reverse_Aux< Cons<typename TODO::Head, DONE>, typename TODO::Tail >::Result;
    typedef Aux Result;
};

template <typename DONE>
struct Reverse_Aux<DONE, Empty>
{
    typedef DONE Result;
};

template <typename LIST>
struct Reverse
{
    using Aux = typename Reverse_Aux<Empty, LIST>::Result;
    typedef Aux Result;
};

template < template<typename> class FUNCTION, typename LIST >
struct ListMap
{
    using H = typename LIST::Head;
    using T = typename LIST::Tail;
    //using F = typename FUNCTION<H>;
    using A = typename FUNCTION<H>::Result;
    //using M = typename ListMap<FUNCTION, T>;
    using B = typename ListMap<FUNCTION, T>::Result;
    typedef Cons<A, B> Result;
    //typedef Cons< FUNCTION<LIST::HEAD>::Result;, ListMap< FUNCTION, LIST::Tail >::Result > Result;
};

template< template<typename> class FUNCTION>
struct ListMap<FUNCTION, Empty>
{
    typedef Empty Result;
};

template <typename X>
struct Inc
{
    typedef UInt<X::Value + 1> Result;
};

template <size_t N, typename LIST>
struct ListTake
{
    using Aux = typename ListTake<N-1, typename LIST::Tail>::Result;
    typedef Cons<typename LIST::Head, Aux> Result;
};

template <typename LIST>
struct ListTake<0, LIST>
{
    typedef Empty Result;
};

template <bool COND, typename NODE, typename LIST>
struct ConditionalCons
{
    typedef Cons<NODE, LIST> Result;
};

template <typename NODE, typename LIST>
struct ConditionalCons<false, NODE, LIST>
{
    typedef LIST Result;
};

template< template<typename> class FUNCTION, typename LIST>
struct ListFilter
{
    static const bool A = FUNCTION<typename LIST::Head>::Result;
    using B = typename ListFilter<FUNCTION, typename LIST::Tail>::Result;
    using C = typename ConditionalCons<A, typename LIST::Head, B>::Result;
    typedef C Result;
};

template< template<typename> class FUNCTION>
struct ListFilter<FUNCTION, Empty>
{
    typedef Empty Result;
};

template <typename X>
struct ListInc
{
    using A = typename Inc<X>::Result;
    using B = typename ListInc<A>::Result;
    typedef Cons<X, B> Result;
};

template <typename X>
struct IsPrime2
{
    static const bool Result = IsPrime<X::Value>::Result;
};

/*
template <bool COND, typename DONE, size_t NB_TODO, typename CANDIDATE>
struct GenPrimesAux2
{
    typedef Cons<CANDIDATE, DONE> ResultList;
    static const size_t ResultNb = NB_TODO - 1;
};

template <typename DONE, size_t NB_TODO, typename CANDIDATE>
struct GenPrimesAux2<false, DONE, NB_TODO, CANDIDATE>
{
    typedef DONE ResultList;
    static const size_t ResultNb = NB_TODO;
};

template <typename DONE, size_t NB_TODO, typename CANDIDATE>
struct GenPrimesAux
{
    static const bool IS_PRIME = IsPrime2<CANDIDATE>::Result;
    typedef GenPrimesAux2<IS_PRIME, 
};
*/

// Case where candidate is not prime
template <size_t NB_TODO, typename CANDIDATE, bool IS_CANDIDATE_PRIME>
struct GenPrimesAux
{
    using A = typename Inc<CANDIDATE>::Result;
    static const bool B = IsPrime2<A>::Result;
    using C = typename GenPrimesAux<NB_TODO, A, B>::Result;
    typedef C Result;
};

template <size_t NB_TODO, typename CANDIDATE>
struct GenPrimesAux<NB_TODO, CANDIDATE, true>
{
    using A = typename Inc<CANDIDATE>::Result;
    static const bool B = IsPrime2<A>::Result;
    using C = typename GenPrimesAux<NB_TODO-1, A, B>::Result;
    typedef Cons<CANDIDATE, C> Result;
};

template <typename CANDIDATE, bool IS_CANDIDATE_PRIME>
struct GenPrimesAux<0, CANDIDATE, IS_CANDIDATE_PRIME>
{
    typedef Empty Result;
};

// Generating the first 'N' primes?
/*
template <typename DONE, size_t NB_TODO, typename CANDIDATE>
struct GenPrimesAux
{
    static const bool C = IsPrime2<CANDIDATE>::Result;
    typedef GenPrimesAux2< C, DONE, NB_TODO, CANDIDATE > Aux;
    using A = typename Aux::ResultList;
    static const size_t N = Aux::ResultNb;
    using B = typename Inc<CANDIDATE>::Result;
    using D = typename GenPrimesAux<A, N, B>::Result;
    typedef D Result;
};

template <typename DONE, typename CANDIDATE>
struct GenPrimesAux<DONE, 0, CANDIDATE>
{
    using A = typename Reverse<DONE>::Result;
    typedef A Result;
};
*/

template <size_t N>
struct GenPrimes
{
    //using A = typename GenPrimesAux<Empty, N, UInt<0> >::Result;
    using A = typename GenPrimesAux<N, UInt<0>, false>::Result;
    typedef A Result;
};

template <typename A, typename B>
struct Adder
{
    typedef UInt<A::Value + B::Value> Result;
};

template <typename LIST>
struct SumList
{
    using H = typename LIST::Head;
    using T = typename LIST::Tail;
    using R = typename SumList<T>::Result;
    using A = typename Adder<H, R>::Result;
    typedef A Result;
};

template <>
struct SumList<Empty>
{
    typedef UInt<0> Result;
};

//typedef ListInc<UInt<0> >::Result ListInts;

typedef Cons< UInt<10>, Empty > L10;
typedef Cons< UInt<9>, L10 > L9;
typedef Cons< UInt<8>, L9 > L8;
typedef Cons< UInt<7>, L8 > L7;
typedef Cons< UInt<6>, L7 > L6;
typedef Cons< UInt<5>, L6 > L5;
typedef Cons< UInt<4>, L5 > L4;
typedef Cons< UInt<3>, L4 > L3;
typedef Cons< UInt<2>, L3 > L2;
typedef Cons< UInt<1>, L2 > L1;
typedef Cons< UInt<0>, L1 > L0;

typedef GenPrimes<10>::Result TenPrimes;


#define N 37

int main(int argc, char* argv[])
{
    std::cout << "IsPrime<N>::Result " << IsPrime<N>::Result << std::endl;
    ListPrimes<50>::Exec();
    std::cout << std::endl;
    L0::print();
    std::cout << std::endl;
    Reverse<L0>::Result::print();
    std::cout << std::endl;
    std::cout << Inc< UInt< 41 > >::Result::Value << std::endl;
    ListMap<Inc, L0>::Result::print();
    std::cout << std::endl;
    ListTake<3, L0>::Result::print();
    std::cout << std::endl;
    ListFilter<IsPrime2, L0>::Result::print();
    std::cout << std::endl;
    TenPrimes::print();
    std::cout << std::endl;
    std::cout << "Sum(TenPrimes): " << SumList<TenPrimes>::Result::Value << std::endl;

    return 0;
}
