#include <boost/yap/expression.hpp>

#include <gtest/gtest.h>

#include <sstream>


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

    number naxpy (number a, number x, number y)
    { return number{a.value * x.value + y.value + 10.0}; }

    inline auto max (int a, int b)
    { return a < b ? b : a; };

    struct tag_type {};

    inline number tag_function (double a, double b)
    { return number{a + b}; }

    struct eval_xform_tag
    {
        decltype(auto) operator() (yap::call_tag, tag_type, number a, double b)
        { return tag_function(a.value, b); }

        int operator() (yap::terminal_tag, tag_type, double a, double b)
        { return 42; }

        char const * operator() ()
        { return "42"; }
    };

    struct empty_xform {};

    struct eval_xform_expr
    {
        decltype(auto) operator() (
            yap::expression<
                yap::expr_kind::call,
                bh::tuple<
                    ref<term<user::tag_type> >,
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
                    ref<term<user::tag_type> >,
                    ref<term<user::number>>,
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
                    ref<term<user::tag_type> >,
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
                    ref<term<user::tag_type> >,
                    ref<term<user::number>>,
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
    };

}


TEST(call_expr, test_call_expr)
{
    using namespace boost::yap::literals;

    {
        auto plus = yap::make_terminal(user::tag_type{});
        auto expr = plus(user::number{13}, 1);

        {
            transform(expr, user::empty_xform{});
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
