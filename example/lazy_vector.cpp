#define BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE
#include <boost/yap/expression.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>


// TODO: Turn this into a test that counts the number of allocations.

template <boost::yap::expr_kind Kind, typename Tuple>
struct lazy_vector_expr;


struct take_nth
{
    boost::yap::terminal<double, lazy_vector_expr>
    operator() (boost::yap::terminal<std::vector<double>, lazy_vector_expr> const & expr);

    std::size_t n;
};

template <boost::yap::expr_kind Kind, typename Tuple>
struct lazy_vector_expr
{
    using this_type = lazy_vector_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;

    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(plus, this_type, ::lazy_vector_expr)
    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(minus, this_type, ::lazy_vector_expr)

    auto operator[] (std::size_t n) const
    { return boost::yap::evaluate(boost::yap::transform(*this, take_nth{n})); }

};
boost::yap::terminal<double, lazy_vector_expr>
take_nth::operator() (boost::yap::terminal<std::vector<double>, lazy_vector_expr> const & expr)
{
    double x = boost::yap::value(expr)[n];
    return boost::yap::make_terminal<lazy_vector_expr, double>(std::move(x));
}

struct lazy_vector :
    lazy_vector_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{
    template <boost::yap::expr_kind Kind, typename Tuple>
    lazy_vector & operator+= (lazy_vector_expr<Kind, Tuple> const & rhs)
    {
        std::vector<double> & this_vec = boost::yap::value(*this);
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
