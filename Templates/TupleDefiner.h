// Assume you have a struct ELT_TYPE_FN, templated on an unsigned int
// type, such that
// typename ELT_TYPE_FN< N >::Type
// defines a type.
//
// TupleDefiner, below, is a helper struct to define, for a static N,
// the tuple
// std::tuple<
//     typename ELT_TYPE_FN< 0 >::Type,
//     typename ELT_TYPE_FN< 1 >::Type,
//     ...
//     typename ELT_TYPE_FN< N >::Type,
// >
//
// That tuple's type is defined by:
// typename TupleDefiner< ELT_TYPE_FN, N >::Type

#include <tuple>

template < template < UIntType > ELT_TYPE_FN, UIntType N >
struct TupleDefiner
{
private:
    typedef typename TupleDefiner< ELT_TYPE_FN, N - 1 >::Type StartType;
    typedef typename ELT_TYPE_FN< N >::Type                   BackType;

public:
    typedef decltype( std::tuple_cat(
        std::declval< StartType >(),
        std::declval< std::tuple< BackType > >()
    ) ) Type;
};

template < template < UIntType > ELT_TYPE_FN >
struct TupleDefiner< ELT_TYPE_FN, 0 >
{
private:
    typedef typename ELT_TYPE_FN< 0 >::Type EltType;

public:
    typedef std::tuple< EltType > Type;
};
