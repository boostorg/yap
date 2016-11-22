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
        using boost::proto17::left;
        using boost::proto17::right;

        if constexpr (Expr::kind == boost::proto17::expr_kind::terminal) {
            return boost::proto17::value(expr)[n];
        } else if constexpr (Expr::kind == boost::proto17::expr_kind::expr_ref) {
            return transform(boost::proto17::value(expr), *this);
        } else if constexpr (Expr::kind == boost::proto17::expr_kind::plus) {
            return transform(left(expr), *this) + transform(right(expr), *this);
        } else if constexpr (Expr::kind == boost::proto17::expr_kind::minus) {
            return transform(left(expr), *this) - transform(right(expr), *this);
        }
    }

    std::size_t n;
};

template <boost::proto17::expr_kind Kind, typename Tuple>
struct lazy_vector_expr
{
    using this_type = lazy_vector_expr<Kind, Tuple>;

    static const boost::proto17::expr_kind kind = Kind;

    Tuple elements;

    template <typename Expr>
    auto operator+ (Expr && rhs)
    {
        using lhs_type = boost::proto17::expression_ref<this_type const &>;
        using rhs_type = boost::proto17::detail::operand_type_t<Expr>;
        using tuple_type = boost::hana::tuple<lhs_type, rhs_type>;
        return lazy_vector_expr<boost::proto17::expr_kind::plus, tuple_type>{
            tuple_type{lhs_type{*this}, rhs_type{rhs}}
        };
    }

    template <typename Expr>
    auto operator- (Expr && rhs)
    {
        using lhs_type = boost::proto17::expression_ref<this_type const &>;
        using rhs_type = boost::proto17::detail::operand_type_t<Expr>;
        using tuple_type = boost::hana::tuple<lhs_type, rhs_type>;
        return lazy_vector_expr<boost::proto17::expr_kind::minus, tuple_type>{
            tuple_type{lhs_type{*this}, rhs_type{rhs}}
        };
    }

    auto operator[] (std::size_t n) const
    { return boost::proto17::transform(*this, eval_nth{n}); }
};

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
