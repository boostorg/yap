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

#if 0 // TODO: Document this verbose form.
    auto eval_expression_as (
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression<
                    bp17::expr_kind::multiplies,
                    bh::tuple<
                        term<number>,
                        term<number>
                    >,
                >,
                term<number>
            >
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
#endif

    decltype(auto) eval_expression_as (
        decltype(term<number>{{0.0}} * number{} + number{}) const & expr,
        boost::hana::basic_type<number>)
    {
        return naxpy(
            expr.left().left().value(),
            expr.left().right().value(),
            expr.right().value()
        );
    }

    decltype(auto) transform_expression (
        decltype(term<number>{{0.0}} * number{} + number{}) const & expr
    ) {
        return naxpy(
            evaluate(expr.left().left()),
            evaluate(expr.left().right()),
            evaluate(expr.right())
        );
    }

}

TEST(user_expression_transform, test_user_expression_transform)
{
    term<user::number> k{{2.0}};

    term<user::number> a{{1.0}};
    term<user::number> x{{42.0}};
    term<user::number> y{{3.0}};

    {
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression<
                    bp17::expr_kind::multiplies,
                    bh::tuple<
                        bp17::expression<
                            bp17::expr_kind::multiplies,
                            bh::tuple<
                                term<user::number>,
                                term<user::number>
                            >
                        >,
                        term<user::number>
                    >
                >,
                term<user::number>
            >
        > expr = term<user::number>{{2.0}} * user::number{1.0} * user::number{42.0} + user::number{3.0};

        user::number result = expr;
        EXPECT_EQ(result.value, 87);
    }

    {
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression<
                    bp17::expr_kind::multiplies,
                    bh::tuple<
                        term<user::number>,
                        term<user::number>
                    >
                >,
                term<user::number>
            >
        > expr = term<user::number>{{1.0}} * user::number{42.0} + user::number{3.0};

        user::number result = expr;
        EXPECT_EQ(result.value, 55);
    }

    {
        bp17::expression<
            bp17::expr_kind::multiplies,
            bh::tuple<
                term<user::number>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        bp17::expression<
                            bp17::expr_kind::multiplies,
                            bh::tuple<
                                term<user::number>,
                                term<user::number>
                            >
                        >,
                        term<user::number>
                    >
                >
            >
        > expr = term<user::number>{{2.0}} * (term<user::number>{{1.0}} * user::number{42.0} + user::number{3.0});

        user::number result = expr;
        EXPECT_EQ(result.value, 110);
    }

    {
        auto expr =
            (term<user::number>{{1.0}} * user::number{42.0} + user::number{3.0}) *
            (term<user::number>{{1.0}} * user::number{42.0} + user::number{3.0}) +
            (term<user::number>{{1.0}} * user::number{42.0} + user::number{3.0});

        user::number result = expr;

        // Note: +10 not done at the top level naxpy opportunity.
        EXPECT_EQ(result.value, 55 * 55 + 55);
    }
}
