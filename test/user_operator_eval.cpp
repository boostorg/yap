#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


namespace user {

    struct number
    {
        explicit operator double () const { return value; }

        double value;
    };

    // User-defined binary-plus!  With weird semantics!
    template <typename A, typename B>
    inline auto eval_plus (A a, B b)
    { return number{a.value - b.value}; }

}

TEST(user_operator_eval, test_user_operator_eval)
{
    term<user::number> unity{{1.0}};
    double d_ = 42.0;
    term<user::number> i{{d_}};
    bp17::expression<
        bp17::expr_kind::plus,
        term<user::number>,
        term<user::number>
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<user::number>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<user::number>,
            term<user::number>
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        double result = unity;
        EXPECT_EQ(result, 1);
    }

    {
        double result = expr;
        EXPECT_EQ(result, -41);
    }

    {
        double result = unevaluated_expr;
        EXPECT_EQ(result, 42);
    }

    {
        double result = (double)evaluate(unity, 5, 6, 7);
        EXPECT_EQ(result, 1);
    }

    {
        double result = (double)evaluate(expr);
        EXPECT_EQ(result, -41);
    }

    {
        double result = (double)evaluate(unevaluated_expr, std::string("15"));
        EXPECT_EQ(result, 42);
    }
}
