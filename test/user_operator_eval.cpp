#define BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>


template <typename T>
using term = boost::yap::terminal<T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


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
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            yap::expression_ref<term<user::number>& >,
            term<user::number>
        >
    > expr = unity + std::move(i);
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            yap::expression_ref<term<user::number>& >,
            yap::expression<
                yap::expr_kind::plus,
                bh::tuple<
                    yap::expression_ref<term<user::number>& >,
                    term<user::number>
                >
            >
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
