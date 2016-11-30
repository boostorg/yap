#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
namespace bh = boost::hana;


namespace user {

    struct number
    {
        double value;

        friend number operator+ (number lhs, number rhs)
        { return number{lhs.value + rhs.value}; }

        friend number operator* (number lhs, number rhs)
        { return number{lhs.value * rhs.value}; }
    };

    number naxpy (number a, number x, number y)
    { return number{a.value * x.value + y.value + 10.0}; }

    template <typename Expr1, typename Expr2, typename Expr3>
    decltype(auto) transform_expression (
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression<
                    bp17::expr_kind::multiplies,
                    bh::tuple<
                        Expr1,
                        Expr2
                    >
                >,
                Expr3
            >
        > const & expr
    ) {
        return naxpy(
            evaluate(expr.left().left()),
            evaluate(expr.left().right()),
            evaluate(expr.right())
        );
    }

}

TEST(user_expression_transform_2, test_user_expression_transform_2)
{
    term<user::number> k{{2.0}};

    term<user::number> a{{1.0}};
    term<user::number> x{{42.0}};
    term<user::number> y{{3.0}};

    {
        auto expr = (a * x + y) * (a * x + y) + (a * x + y);

        user::number result = expr;

        EXPECT_EQ(result.value, 55 * 55 + 55 + 10);
    }
}
