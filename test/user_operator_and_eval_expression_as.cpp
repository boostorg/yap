#define BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE 1
#include <boost/yap/expression.hpp>

#include <gtest/gtest.h>


template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

template <typename T>
using ref = boost::yap::expression_ref<boost::yap::expression, T>;

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

    template <typename E, typename ...T>
    constexpr auto eval_expression_as (
        E const & expr,
        boost::hana::basic_type<user::number>,
        T &&... args)
    {
        user::number const x =
            yap::detail::default_eval_expr(expr, static_cast<T &&>(args)...);
        return user::number{x.value + 5.0};
    }

}

TEST(user_eval_expression_as, test_user_eval_expression_as)
{
    term<user::number> unity{{1.0}};
    double d_ = 42.0;
    term<user::number> i{{d_}};
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            ref<term<user::number>& >,
            term<user::number>
        >
    > expr = unity + std::move(i);
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            ref<term<user::number>& >,
            yap::expression<
                yap::expr_kind::plus,
                bh::tuple<
                    ref<term<user::number>& >,
                    term<user::number>
                >
            >
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        user::number result = unity;
        EXPECT_EQ(result.value, 6);
    }

    {
        user::number result = expr;
        EXPECT_EQ(result.value, -36);
    }

    {
        user::number result = unevaluated_expr;
        EXPECT_EQ(result.value, 47);
    }

    {
        user::number result = evaluate(unity, 5, 6, 7);
        EXPECT_EQ(result.value, 1);
    }

    {
        double result = (double)evaluate(expr);
        EXPECT_EQ(result, -41);
    }

    {
        double result = (double)evaluate(unevaluated_expr, std::string("15"));
        EXPECT_EQ(result, 42);
    }

    {
        user::number result = yap::evaluate_as<user::number>(unity, 5, 6, 7);
        EXPECT_EQ(result.value, 6);
    }

    {
        user::number result = yap::evaluate_as<user::number>(expr);
        EXPECT_EQ(result.value, -36);
    }

    {
        user::number result = yap::evaluate_as<user::number>(unevaluated_expr, std::string("15"));
        EXPECT_EQ(result.value, 47);
    }
}
