#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


TEST(default_eval, default_eval)
{
    term<double> unity{1.0};
    int i_ = 42;
    term<int &&> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::minus,
        term<double>,
        term<int &&>
    > expr = unity - std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        bp17::expression<
            bp17::expr_kind::minus,
            term<double>,
            term<int &&>
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
        EXPECT_EQ(result, -40);
    }

    {
        double result = evaluate(unity, 5, 6, 7);
        EXPECT_EQ(result, 1);
    }

    {
        double result = evaluate(expr);
        EXPECT_EQ(result, -41);
    }

    {
        double result = evaluate(unevaluated_expr, std::string("15"));
        EXPECT_EQ(result, -40);
    }
}
