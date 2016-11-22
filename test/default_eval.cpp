#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
namespace bh = boost::hana;


TEST(default_eval, default_eval)
{
    term<double> unity{1.0};
    int i_ = 42;
    term<int &&> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::minus,
        bh::tuple<
            boost::proto17::expression_ref<term<double> &>,
            term<int &&>
        >
    > expr = unity - std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            boost::proto17::expression_ref<term<double> &>,
            bp17::expression<
                bp17::expr_kind::minus,
                bh::tuple<
                    boost::proto17::expression_ref<term<double> &>,
                    term<int &&>
                >
            >
        >
    > unevaluated_expr_1 = unity + std::move(expr);

    bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            boost::proto17::expression_ref<term<double> &>,
            boost::proto17::expression_ref<term<double> &>
        >
    > unevaluated_expr_2 = unity + unity;

    term<double> const const_unity{1.0};
    bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            boost::proto17::expression_ref<term<double> &>,
            boost::proto17::expression_ref<term<double> const &>
        >
    > unevaluated_expr_3 = unity + const_unity;

    {
        double result = unity;
        EXPECT_EQ(result, 1);
    }

    {
        double result = expr;
        EXPECT_EQ(result, -41);
    }

    {
        double result = unevaluated_expr_1;
        EXPECT_EQ(result, -40);
    }

    {
        double result = unevaluated_expr_2;
        EXPECT_EQ(result, 2);
    }

    {
        double result = unevaluated_expr_3;
        EXPECT_EQ(result, 2);
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
        double result = evaluate(unevaluated_expr_1, std::string("15"));
        EXPECT_EQ(result, -40);
    }

    {
        double result = evaluate(unevaluated_expr_2, std::string("15"));
        EXPECT_EQ(result, 2);
    }

    {
        double result = evaluate(unevaluated_expr_3, std::string("15"));
        EXPECT_EQ(result, 2);
    }
}
