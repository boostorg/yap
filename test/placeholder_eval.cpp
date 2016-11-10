#include "expression.hpp"

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


TEST(placeholder_eval, test_placeholder_eval)
{
    using namespace boost::proto17::literals;

    bp17::placeholder<2> p2 = 2_p;
    int i_ = 42;
    term<int> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::plus,
        bp17::placeholder<2>,
        term<int>
    > expr = p2 + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        bp17::placeholder<2>,
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<2>,
            term<int>
        >
    > unevaluated_expr = p2 + std::move(expr);

    {
        double result = evaluate(p2, 5, 6, 7);
        EXPECT_EQ(result, 7);
    }

    {
        double result = evaluate(expr, std::string("15"), 3, 1);
        EXPECT_EQ(result, 43);
    }

    {
        double result = evaluate(unevaluated_expr, std::string("15"), 2, 3);
        EXPECT_EQ(result, 48);
    }
}
