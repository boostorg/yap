#include "expression.hpp"

#include <gtest/gtest.h>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


namespace user {

    struct number
    {
        double value;

        friend number operator+ (number lhs, number rhs)
        { return number{lhs.value + rhs.value}; }

        friend number operator- (number lhs, number rhs)
        { return number{lhs.value - rhs.value}; }

        friend number operator* (number lhs, number rhs)
        { return number{lhs.value * rhs.value}; }
    };

    number naxpy (number a, number x, number y)
    { return number{a.value * x.value + y.value + 10.0}; }

    struct eval_xform
    {
        decltype(auto) operator() (term<user::number> const & expr)
        { return expr.value(); }
    };

    struct plus_to_minus_xform
    {
        template <typename Expr1, typename Expr2>
        decltype(auto) operator() (bp17::expression<bp17::expr_kind::plus, Expr1, Expr2> const & expr)
        { return bp17::make_expression<bp17::expr_kind::minus>(expr.left(), expr.right()); }
    };

    decltype(auto) naxpy_eager_nontemplate_xform (
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::expression<
                bp17::expr_kind::multiplies,
                term<user::number>,
                term<user::number>
            >,
            term<user::number>
        > const & expr
    ) {
        auto a = evaluate(expr.left().left());
        auto x = evaluate(expr.left().right());
        auto y = evaluate(expr.right());
        return bp17::make_terminal(naxpy(a, x, y));
    }

    decltype(auto) naxpy_lazy_nontemplate_xform (
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::expression<
                bp17::expr_kind::multiplies,
                term<user::number>,
                term<user::number>
            >,
            term<user::number>
        > const & expr
    ) {
        auto a = expr.left().left();
        auto x = expr.left().right();
        auto y = expr.right();
        return bp17::make_terminal(naxpy)(a, x, y);
    }

    struct naxpy_xform
    {
        template <typename Expr1, typename Expr2, typename Expr3>
        decltype(auto) operator() (
            bp17::expression<
                bp17::expr_kind::plus,
                bp17::expression<
                    bp17::expr_kind::multiplies,
                    Expr1,
                    Expr2
                >,
                Expr3
            > const & expr
        ) {
            auto a = transform(expr.left().left(), naxpy_xform{});
            auto x = transform(expr.left().right(), naxpy_xform{});
            auto y = transform(expr.right(), naxpy_xform{});
            return bp17::make_terminal(naxpy)(a, x, y);
        }
    };

}

TEST(user_expression_transform_3, test_user_expression_transform_3)
{
    term<user::number> a{{1.0}};
    term<user::number> x{{42.0}};
    term<user::number> y{{3.0}};

    {
        auto expr = a;
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 1);
        }

        auto transformed_expr = transform(expr, user::eval_xform{});
        {
            EXPECT_EQ(transformed_expr.value, 1);
        }
    }

    {
        auto expr = x + y;
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 45);
        }

        auto transformed_expr = transform(expr, user::plus_to_minus_xform{});
        {
            user::number result = evaluate(transformed_expr);
            EXPECT_EQ(result.value, 39);
        }
    }

    {
        auto expr = a * x + y;
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 45);
        }

        auto transformed_expr = transform(expr, user::naxpy_eager_nontemplate_xform);
        {
            user::number result = evaluate(transformed_expr);
            EXPECT_EQ(result.value, 55);
        }
    }

    {
        auto expr = a + (a * x + y);
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 46);
        }

        auto transformed_expr = transform(expr, user::naxpy_eager_nontemplate_xform);
        {
            user::number result = evaluate(transformed_expr);
            EXPECT_EQ(result.value, 56);
        }
    }

    {
        auto expr = a * x + y;
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 45);
        }

        auto transformed_expr = transform(expr, user::naxpy_lazy_nontemplate_xform);
        {
            user::number result = evaluate(transformed_expr);
            EXPECT_EQ(result.value, 55);
        }
    }

    {
        auto expr = a + (a * x + y);
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 46);
        }

        auto transformed_expr = transform(expr, user::naxpy_lazy_nontemplate_xform);
        {
            user::number result = evaluate(transformed_expr);
            EXPECT_EQ(result.value, 56);
        }
    }

    {
        auto expr = (a * x + y) * (a * x + y) + (a * x + y);
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 45 * 45 + 45);
        }

        auto transformed_expr = transform(expr, user::naxpy_xform{});
        {
            user::number result = evaluate(transformed_expr);
            EXPECT_EQ(result.value, 55 * 55 + 55 + 10);
        }
    }
}

auto double_to_float (term<double> expr)
{ return term<float>{(float)expr.value()}; }

auto check_unique_ptrs_equal_7 (term<std::unique_ptr<int>> && expr)
{
    using namespace boost::hana::literals;
    EXPECT_EQ(*expr.elements[0_c], 7);
    return std::move(expr);
}

TEST(move_only, test_user_expression_transform_3)
{
    term<double> unity{1.0};
    term<std::unique_ptr<int>> i{new int{7}};
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        term<std::unique_ptr<int>>
    > expr_1 = unity + std::move(i);

    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<std::unique_ptr<int>>
        >
    > expr_2 = unity + std::move(expr_1);

    auto transformed_expr = transform(std::move(expr_2), double_to_float);

    transform(std::move(transformed_expr), check_unique_ptrs_equal_7);
}
