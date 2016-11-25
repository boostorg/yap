#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>


// TODO: Turn this into a test that counts the number of allocations.

template <boost::proto17::expr_kind Kind, typename Tuple>
struct lazy_vector_expr;


struct take_nth
{
    boost::proto17::terminal<double, lazy_vector_expr>
    operator() (boost::proto17::terminal<std::vector<double>, lazy_vector_expr> const & expr);

    std::size_t n;
};

template <boost::proto17::expr_kind Kind, typename Tuple>
struct lazy_vector_expr
{
    using this_type = lazy_vector_expr<Kind, Tuple>;

    static const boost::proto17::expr_kind kind = Kind;

    Tuple elements;

    BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(plus, this_type, ::lazy_vector_expr)
    BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(minus, this_type, ::lazy_vector_expr)

    auto operator[] (std::size_t n) const
    { return boost::proto17::evaluate(boost::proto17::transform(*this, take_nth{n})); }

};
boost::proto17::terminal<double, lazy_vector_expr>
take_nth::operator() (boost::proto17::terminal<std::vector<double>, lazy_vector_expr> const & expr)
{
    double const x = boost::proto17::value(expr)[n];
    return boost::proto17::make_terminal<lazy_vector_expr, double>(x);
}

struct lazy_vector :
    lazy_vector_expr<
        boost::proto17::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{
    template <boost::proto17::expr_kind Kind, typename Tuple>
    lazy_vector & operator+= (lazy_vector_expr<Kind, Tuple> const & rhs)
    {
        std::vector<double> & this_vec = boost::proto17::value(*this);
        for (int i = 0, size = (int)this_vec.size(); i < size; ++i) {
            this_vec[i] += rhs[i];
        }
        return *this;
    }
};

int main ()
{
    lazy_vector v1{{std::vector<double>(4, 1.0)}};
    lazy_vector v2{{std::vector<double>(4, 2.0)}};
    lazy_vector v3{{std::vector<double>(4, 3.0)}};

    double d1 = (v2 + v3)[2];
    std::cout << d1 << "\n";

    v1 += v2 - v3;
    std::cout << '{' << v1[0] << ',' << v1[1]
              << ',' << v1[2] << ',' << v1[3] << '}' << "\n";

    // This expression is disallowed because it does not conform
    // to the implicit grammar.
    // (v2 + v3) += v1;

    return 0;
}
