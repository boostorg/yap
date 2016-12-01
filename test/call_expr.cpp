#define BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>

#include <sstream>


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

    number naxpy (number a, number x, number y)
    { return number{a.value * x.value + y.value + 10.0}; }

    inline auto max (int a, int b)
    { return a < b ? b : a; };

    struct tag_type {};

    inline number tag_function (double a, double b)
    { return number{a + b}; }

    struct eval_xform_tag
    {
        decltype(auto) operator() (yap::call_tag, tag_type, double a, double b)
        { return tag_function(a, b); }
    };

    struct empty_xform {};

    struct eval_xform_expr
    {
        decltype(auto) operator() (
            yap::expression<
                yap::expr_kind::call,
                bh::tuple<
                    yap::expression_ref<term<user::tag_type> >,
                    term<user::number>,
                    term<int>
                >
            > const & expr
        ) {
            using namespace boost::hana::literals;
            return tag_function(
                (double)yap::value(expr.elements[1_c]).value,
                (double)yap::value(expr.elements[2_c])
            );
        }

        decltype(auto) operator() (
            yap::expression<
                yap::expr_kind::call,
                bh::tuple<
                    yap::expression_ref<term<user::tag_type> >,
                    yap::expression_ref<term<user::number>>,
                    term<int>
                >
            > const & expr
        ) {
            using namespace boost::hana::literals;
            return tag_function(
                (double)yap::deref(expr.elements[1_c]).value,
                (double)yap::value(expr.elements[2_c])
            );
        }
    };

    struct eval_xform_both
    {
        decltype(auto) operator() (yap::call_tag, tag_type, double a, double b)
        {
            // TODO: Document that this differs from the behavior for
            // non-call_tag tagged overloads, which will always be preferred
            // to expr-overloads.  Moreover, document that mixing expr- and
            // tag-based overloads is usually a bad idea.
            throw std::logic_error("Oops!  Picked the wrong overload!");
            return tag_function(a, b);
        }

        decltype(auto) operator() (
            yap::expression<
                yap::expr_kind::call,
                bh::tuple<
                    yap::expression_ref<term<user::tag_type> >,
                    term<user::number>,
                    term<int>
                >
            > const & expr
        ) {
            using namespace boost::hana::literals;
            return tag_function(
                (double)yap::value(expr.elements[1_c]).value,
                (double)yap::value(expr.elements[2_c])
            );
        }

        decltype(auto) operator() (
            yap::expression<
                yap::expr_kind::call,
                bh::tuple<
                    yap::expression_ref<term<user::tag_type> >,
                    yap::expression_ref<term<user::number>>,
                    term<int>
                >
            > const & expr
        ) {
            using namespace boost::hana::literals;
            return tag_function(
                (double)yap::deref(expr.elements[1_c]).value,
                (double)yap::value(expr.elements[2_c])
            );
        }
    };

    template <typename ...T>
    inline auto eval_call (tag_type, T && ...t)
    {
        if constexpr (sizeof...(T) == 2u) {
            return tag_function((double)t...);
        } else {
            assert(!"Unhandled case in eval_call()");
            return;
        }
    }

}


TEST(call_expr, test_call_expr)
{
    using namespace boost::yap::literals;

    {
        yap::expression<
            yap::expr_kind::call,
            bh::tuple<
                yap::placeholder<1>,
                yap::placeholder<2>,
                yap::placeholder<3>
            >
        > expr = 1_p(2_p, 3_p);

        {
            auto min = [] (int a, int b) { return a < b ? a : b; };
            int result = evaluate(expr, min, 3, 7);
            EXPECT_EQ(result, 3);
        }

        {
            int result = evaluate(expr, &user::max, 3, 7);
            EXPECT_EQ(result, 7);
        }

        {
            int result = evaluate(expr, user::max, 3, 7);
            EXPECT_EQ(result, 7);
        }
    }

    {
        auto min_lambda = [] (int a, int b) { return a < b ? a : b; };

        {
            auto min = yap::make_terminal(min_lambda);
            auto expr = min(1_p, 2_p);

            {
                int result = evaluate(expr, 3, 7);
                EXPECT_EQ(result, 3);
            }
        }

        {
            term<decltype(min_lambda)> min = {{min_lambda}};
            auto expr = min(1_p, 2_p);

            {
                int result = evaluate(expr, 3, 7);
                EXPECT_EQ(result, 3);
            }
        }
    }

    {
        struct min_function_object_t
        {
            auto operator() (int a, int b) const { return a < b ? a : b; }
        };

        min_function_object_t min_function_object;

        {
            term<min_function_object_t &> min = yap::make_terminal(min_function_object);
            auto expr = min(1_p, 2_p);

            {
                using namespace boost::hana::literals;
                int result = evaluate(expr, 3, 7);
                EXPECT_EQ(result, 3);
            }
        }

        {
            term<min_function_object_t> min = {{min_function_object}};
            auto expr = min(1_p, 2_p);

            {
                int result = evaluate(expr, 3, 7);
                EXPECT_EQ(result, 3);
            }
        }

        {
            auto min = yap::make_terminal(min_function_object_t{});
            auto expr = min(1_p, 2_p);

            {
                int result = evaluate(expr, 3, 7);
                EXPECT_EQ(result, 3);
            }
        }

        {
            term<min_function_object_t> min = {{min_function_object_t{}}};
            auto expr = min(1_p, 2_p);

            {
                int result = evaluate(expr, 3, 7);
                EXPECT_EQ(result, 3);
            }
        }

    }

    {
        auto min_lambda = [] (int a, int b) { return a < b ? a : b; };

        {
            auto min = yap::make_terminal(min_lambda);
            auto expr = min(0, 1);

            {
                int result = evaluate(expr);
                EXPECT_EQ(result, 0);
            }
        }

        {
            term<decltype(min_lambda)> min = {{min_lambda}};
            yap::expression<
                yap::expr_kind::call,
                bh::tuple<
                    yap::expression_ref<term<decltype(min_lambda)>& >,
                    term<int>,
                    term<int>
                >
            > expr = min(0, 1);

            {
                int result = evaluate(expr);
                EXPECT_EQ(result, 0);
            }
        }
    }

    {
        {
            auto plus = yap::make_terminal(user::tag_type{});
            auto expr = plus(user::number{13}, 1);

            {
                user::number result = expr;
                EXPECT_EQ(result.value, 14);
            }
        }

        {
            term<user::tag_type> plus = {{user::tag_type{}}};
            yap::expression<
                yap::expr_kind::call,
                bh::tuple<
                    yap::expression_ref<term<user::tag_type>& >,
                    term<int>,
                    term<int>
                >
            > expr = plus(0, 1);

            {
                user::number result = expr;
                EXPECT_EQ(result.value, 1);
            }
        }

        {
            auto plus = yap::make_terminal(user::tag_type{});
            auto expr = plus(user::number{13}, 1);

            {
                auto transformed_expr = transform(expr, user::empty_xform{});
                user::number result = transformed_expr;
                EXPECT_EQ(result.value, 14);
            }

            {
                user::number result = transform(expr, user::eval_xform_tag{});
                EXPECT_EQ(result.value, 14);
            }

            {
                user::number result = transform(expr, user::eval_xform_expr{});
                EXPECT_EQ(result.value, 14);
            }

            {
                user::number result = transform(expr, user::eval_xform_both{});
                EXPECT_EQ(result.value, 14);
            }
        }

        {
            auto plus = yap::make_terminal(user::tag_type{});
            auto thirteen = yap::make_terminal(user::number{13});
            auto expr = plus(thirteen, 1);

            {
                user::number result = transform(expr, user::eval_xform_tag{});
                EXPECT_EQ(result.value, 14);
            }

            {
                user::number result = transform(expr, user::eval_xform_expr{});
                EXPECT_EQ(result.value, 14);
            }

            {
                user::number result = transform(expr, user::eval_xform_both{});
                EXPECT_EQ(result.value, 14);
            }
        }
    }

    {
        term<user::number> a{{1.0}};
        term<user::number> x{{42.0}};
        term<user::number> y{{3.0}};
        auto n = yap::make_terminal(user::naxpy);

        auto expr = n(a, x, y);
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 55);
        }
    }
}
