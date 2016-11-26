#include "expression.hpp"

#include <array>
#include <iostream>


template <boost::proto17::expr_kind Kind, typename Tuple>
struct tarray_expr;


struct take_nth
{
    boost::proto17::terminal<int, tarray_expr>
    operator() (boost::proto17::terminal<std::array<int, 3>, tarray_expr> const & expr);

    std::size_t n;
};

template <boost::proto17::expr_kind Kind, typename Tuple>
struct tarray_expr
{
    static_assert(
        Kind != boost::proto17::expr_kind::terminal ||
        std::is_same<Tuple, boost::hana::tuple<int>>{} ||
        std::is_same<Tuple, boost::hana::tuple<std::array<int, 3>>>{}
    );

    using this_type = tarray_expr<Kind, Tuple>;

    static const boost::proto17::expr_kind kind = Kind;

    Tuple elements;

    BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(plus, this_type, ::tarray_expr)
    BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(minus, this_type, ::tarray_expr)
    BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(multiplies, this_type, ::tarray_expr)
    BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(divides, this_type, ::tarray_expr)

    int operator[] (std::size_t n) const
    { return boost::proto17::evaluate(boost::proto17::transform(*this, take_nth{n})); }
};

BOOST_PROTO17_USER_NON_MEMBER_BINARY_OPERATOR(plus, ::tarray_expr)
BOOST_PROTO17_USER_NON_MEMBER_BINARY_OPERATOR(minus, ::tarray_expr)
BOOST_PROTO17_USER_NON_MEMBER_BINARY_OPERATOR(multiplies, ::tarray_expr)
BOOST_PROTO17_USER_NON_MEMBER_BINARY_OPERATOR(divides, ::tarray_expr)


boost::proto17::terminal<int, tarray_expr>
take_nth::operator() (boost::proto17::terminal<std::array<int, 3>, tarray_expr> const & expr)
{
    int x = boost::proto17::value(expr)[n];
    return boost::proto17::make_terminal<tarray_expr>(std::move(x));
}


std::ostream & operator<< (std::ostream & os, boost::proto17::terminal<int, tarray_expr> expr)
{ return os << '{' << boost::proto17::value(expr) << '}'; }

std::ostream & operator<< (std::ostream & os, boost::proto17::terminal<std::array<int, 3>, tarray_expr> expr)
{
    std::array<int, 3> const & a = boost::proto17::value(expr);
    return os << '{' << a[0] << ", " << a[1] << ", " << a[2] << '}';
}

template <typename Tuple>
std::ostream & operator<< (std::ostream & os, tarray_expr<boost::proto17::expr_kind::expr_ref, Tuple> const & expr)
{ return os << boost::proto17::value(expr); }

template <boost::proto17::expr_kind Kind, typename Tuple>
std::ostream & operator<< (std::ostream & os, tarray_expr<Kind, Tuple> const & expr)
{
    if (Kind == boost::proto17::expr_kind::plus || Kind == boost::proto17::expr_kind::minus)
        os << '(';
    os << boost::proto17::left(expr) << " " << op_string(Kind) << " " << boost::proto17::right(expr);
    if (Kind == boost::proto17::expr_kind::plus || Kind == boost::proto17::expr_kind::minus)
        os << ')';
    return os;
}


struct TArray :
    tarray_expr<
        boost::proto17::expr_kind::terminal,
        boost::hana::tuple<std::array<int, 3>>
    >
{
    explicit TArray (int i = 0, int j = 0, int k = 0)
    {
        (*this)[0] = i;
        (*this)[1] = j;
        (*this)[2] = k;
    }

    explicit TArray (std::array<int, 3> a)
    {
        (*this)[0] = a[0];
        (*this)[1] = a[1];
        (*this)[2] = a[2];
    }

    int & operator[] (std::ptrdiff_t i)
    { return boost::proto17::value(*this)[i]; }

    int const & operator[] (std::ptrdiff_t i) const
    { return boost::proto17::value(*this)[i]; }

    TArray & operator= (int i)
    { return assign(TArray(i)); }

    template <typename Expr>
    TArray & operator= (Expr const & expr)
    {
        if constexpr (Expr::kind == boost::proto17::expr_kind::terminal) {
            return assign(TArray(boost::proto17::value(expr)));
        } else {
            return assign(expr);
        }
    }

    template <typename Expr>
    TArray & printAssign (Expr const & expr)
    {
        *this = expr;
        std::cout << *this << " = " << expr << std::endl;
        return *this;
    }

private:
    template <typename Expr>
    TArray & assign (Expr const & expr)
    {
        (*this)[0] = expr[0];
        (*this)[1] = expr[1];
        (*this)[2] = expr[2];
        return *this;
    }
};


int main()
{
    TArray a(3,1,2);

    TArray b;

    std::cout << a << std::endl;
    std::cout << b << std::endl;

    b[0] = 7; b[1] = 33; b[2] = -99;

    TArray c(a);

    std::cout << c << std::endl;

    a = 0;

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;

    a = b + c;

    std::cout << a << std::endl;

    a.printAssign(b+c*(b + 3*c));

    return 0;
}
