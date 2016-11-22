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
    using namespace boost::proto17::literals;

    {
        bp17::expression<
            bp17::expr_kind::call,
            bh::tuple<
                bp17::placeholder<1>,
                bp17::placeholder<2>,
                bp17::placeholder<3>
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
            auto min = bp17::make_terminal(min_lambda);
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
            term<min_function_object_t &> min = bp17::make_terminal(min_function_object);
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
            auto min = bp17::make_terminal(min_function_object_t{});
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
            auto min = bp17::make_terminal(min_lambda);
            auto expr = min(0, 1);

            {
                int result = evaluate(expr);
                EXPECT_EQ(result, 0);
            }
        }

        {
            term<decltype(min_lambda)> min = {{min_lambda}};
            bp17::expression<
                bp17::expr_kind::call,
                bh::tuple<
                    term<decltype(min_lambda)>,
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
            auto plus = bp17::make_terminal(user::tag_type{});
            auto expr = plus(user::number{13}, 1);

            {
                user::number result = expr;
                EXPECT_EQ(result.value, 14);
            }
        }

        {
            term<user::tag_type> plus = {{user::tag_type{}}};
            bp17::expression<
                bp17::expr_kind::call,
                bh::tuple<
                    term<user::tag_type>,
                    term<int>,
                    term<int>
                >
            > expr = plus(0, 1);

            {
                user::number result = expr;
                EXPECT_EQ(result.value, 1);
            }
        }
    }

    {
        term<user::number> a{{1.0}};
        term<user::number> x{{42.0}};
        term<user::number> y{{3.0}};
        auto n = bp17::make_terminal(user::naxpy);

        auto expr = n(a, x, y);
        {
            user::number result = evaluate(expr);
            EXPECT_EQ(result.value, 55);
        }
    }
}
