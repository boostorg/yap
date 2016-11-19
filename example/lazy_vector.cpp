#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>


struct eval_nth
{
    template <typename Expr>
    auto operator() (Expr const & expr)
    {
        using boost::proto17::transform;
        using boost::proto17::value;
        using boost::proto17::left;
        using boost::proto17::right;

        if constexpr (Expr::kind == boost::proto17::expr_kind::terminal) {
            return value(expr)[n];
        } else if constexpr (Expr::kind == boost::proto17::expr_kind::plus) {
            return transform(left(expr), *this) + transform(right(expr), *this);
        } else if constexpr (Expr::kind == boost::proto17::expr_kind::minus) {
            return transform(left(expr), *this) - transform(right(expr), *this);
        }
    }

    std::size_t n;
};

template <boost::proto17::expr_kind Kind, typename ...T>
struct lazy_vector_expr
{
    using this_type = lazy_vector_expr<Kind, T...>;

    static const boost::proto17::expr_kind kind = Kind;

    boost::hana::tuple<T...> elements;

    template <boost::proto17::expr_kind Kind2, typename ...U>
    auto operator+ (lazy_vector_expr<Kind2, U...> const & rhs)
    {
        using rhs_type = lazy_vector_expr<Kind2, U...>;
        return lazy_vector_expr<boost::proto17::expr_kind::plus, this_type, rhs_type>{
            boost::hana::tuple<this_type, rhs_type>{*this, rhs}
        };
    }

    template <boost::proto17::expr_kind Kind2, typename ...U>
    auto operator- (lazy_vector_expr<Kind2, U...> const & rhs)
    {
        using rhs_type = lazy_vector_expr<Kind2, U...>;
        return lazy_vector_expr<boost::proto17::expr_kind::minus, this_type, rhs_type>{
            boost::hana::tuple<this_type, rhs_type>{*this, rhs}
        };
    }

    auto operator[] (std::size_t n) const
    { return boost::proto17::transform(*this, eval_nth{n}); }
};

struct lazy_vector :
    lazy_vector_expr<
        boost::proto17::expr_kind::terminal,
        std::vector<double> // TODO: Use a reference?
    >
{
    template <boost::proto17::expr_kind Kind, typename ...T>
    auto operator+= (lazy_vector_expr<Kind, T...> const & rhs)
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

    // TODO: Restore reference preservation when building expression trees.
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
