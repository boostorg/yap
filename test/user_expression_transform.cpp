#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


namespace user_2 {

    struct number
    {
        double value;
    };

    number naxpy (number a, number x, number y)
    { return number{a.value * x.value + y.value + 100.0}; }

#if 0
    auto eval_expression_as (
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::expression<
                bp17::expr_kind::multiplies,
                term<number>,
                term<number>
            >,
            term<number>
        > const & expr,
        boost::hana::basic_type<number>)
    {
        using namespace boost::hana::literals;
        return naxpy(
            expr.elements[0_c].elements[0_c].elements[0_c],
            expr.elements[0_c].elements[1_c].elements[0_c],
            expr.elements[1_c].elements[0_c]
        );
    }
#else
    auto eval_expression_as (
        decltype(term<number>{{0.0}} * number{} + number{}) const & expr,
        boost::hana::basic_type<number>,
        boost::hana::tuple<>)
    {
        return naxpy(
            expr.left().left().value(),
            expr.left().right().value(),
            expr.right().value()
        );
    }
#endif

}

TEST(user_expression_transform, test_user_expression_transform)
{
    term<user_2::number> a{{1.0}};
    term<user_2::number> x{{42.0}};
    term<user_2::number> y{{3.0}};

    bp17::expression<
        bp17::expr_kind::plus,
        bp17::expression<
            bp17::expr_kind::multiplies,
            term<user_2::number>,
            term<user_2::number>
        >,
        term<user_2::number>
    > expr = a * x + y;

    // TODO: This was an error (user:: vs. user_2::).  Document for users that
    // they should catch an expression in an auto var to diagnose these sorts
    // of things.
#if 0
    bp17::expression<
        bp17::expr_kind::multiplies,
        term<user::number>,
        term<user::number>
    > expr = a * x;
#endif

    user_2::number result = expr;
    EXPECT_EQ(result.value, 145);
}
