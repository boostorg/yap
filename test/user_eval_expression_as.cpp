#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
namespace bh = boost::hana;


namespace user {

    struct number
    {
        explicit operator double () const { return value; }

        friend number operator+ (number lhs, number rhs)
        { return number{lhs.value + rhs.value}; }

        double value;
    };

    template <typename E, typename ...T>
    constexpr auto eval_expression_as (
        E const & expr,
        boost::hana::basic_type<user::number>,
        T &&... args)
    {
        return static_cast<user::number>(
            bp17::detail::default_eval_expr(expr, static_cast<T &&>(args)...)
        );
    }

}

TEST(user_eval_expression_as, test_user_eval_expression_as)
{
    term<user::number> unity{{1.0}};
    double d_ = 42.0;
    term<user::number> i{{d_}};
    bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            term<user::number>,
            term<user::number>
        >
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            term<user::number>,
            bp17::expression<
                bp17::expr_kind::plus,
                bh::tuple<
                    term<user::number>,
                    term<user::number>
                >
            >
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        user::number result = unity;
        EXPECT_EQ(result.value, 1);
    }

    {
        user::number result = expr;
        EXPECT_EQ(result.value, 43);
    }

    {
        user::number result = unevaluated_expr;
        EXPECT_EQ(result.value, 44);
    }

    {
        user::number result = evaluate(unity, 5, 6, 7);
        EXPECT_EQ(result.value, 1);
    }

    {
        double result = (double)evaluate(expr);
        EXPECT_EQ(result, 43);
    }

    {
        double result = (double)evaluate(unevaluated_expr, std::string("15"));
        EXPECT_EQ(result, 44);
    }

    {
        user::number result = bp17::evaluate_as<user::number>(unity, 5, 6, 7);
        EXPECT_EQ(result.value, 1);
    }

    {
        user::number result = bp17::evaluate_as<user::number>(expr);
        EXPECT_EQ(result.value, 43);
    }

    {
        user::number result = bp17::evaluate_as<user::number>(unevaluated_expr, std::string("15"));
        EXPECT_EQ(result.value, 44);
    }
}
