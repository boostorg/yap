#include "expression.hpp"

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::yap::terminal<T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


TEST(placeholder_eval, test_placeholder_eval)
{
    using namespace boost::yap::literals;

    yap::placeholder<3> p3 = 3_p;
    int i_ = 42;
    term<int> i{std::move(i_)};
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            yap::expression_ref<yap::placeholder<3> &>,
            term<int>
        >
    > expr = p3 + std::move(i);
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            yap::expression_ref<yap::placeholder<3> &>,
            yap::expression<
                yap::expr_kind::plus,
                bh::tuple<
                    yap::expression_ref<yap::placeholder<3> &>,
                    term<int>
                >
            >
        >
    > unevaluated_expr = p3 + std::move(expr);

    {
        double result = evaluate(p3, 5, 6, 7);
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
