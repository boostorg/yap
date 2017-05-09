#include <boost/yap/expression.hpp>

#include <gtest/gtest.h>


template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

template <typename T>
using ref = boost::yap::expression_ref<boost::yap::expression, T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    static boost::yap::expr_kind const kind = Kind;

    Tuple elements;
    
    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(plus, ::user_expr)
};

template <typename T>
using user_term = boost::yap::terminal<user_expr, T>;

template <typename T>
using user_ref = boost::yap::expression_ref<user_expr, T>;


TEST(expression, test_deref)
{
    term<double> unity = {{1.0}};
    using plus_expr_type = yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            ref<term<double> &>,
            term<int>
        >
    >;

    plus_expr_type plus_expr = unity + term<int>{{1}};

    {
        ref<term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(std::move(ref))), term<double> &>::value));
    }

    {
        ref<term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), term<double> &>::value));
    }

    {
        ref<term<double> &> const ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), term<double> &>::value));
    }

    {
        term<double> const unity = {{1.0}};
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                ref<term<double> const &>,
                term<int>
            >
        > plus_expr = unity + term<int>{{1}};

        {
            ref<term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(std::move(ref))), term<double> const &>::value));
        }

        {
            ref<term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), term<double> const &>::value));
        }

        {
            ref<term<double> const &> const ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), term<double> const &>::value));
        }
    }
}

TEST(user_expr, test_deref)
{
    user_term<double> unity = {{1.0}};
    using plus_expr_type = user_expr<
        yap::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_term<int>
        >
    >;

    plus_expr_type plus_expr = unity + user_term<int>{{1}};

    {
        user_ref<user_term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(std::move(ref))), user_term<double> &>::value));
    }

    {
        user_ref<user_term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), user_term<double> &>::value));
    }

    {
        user_ref<user_term<double> &> const ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), user_term<double> &>::value));
    }

    {
        user_term<double> const unity = {{1.0}};
        user_expr<
            yap::expr_kind::plus,
            bh::tuple<
                user_ref<user_term<double> const &>,
                user_term<int>
            >
        > plus_expr = unity + user_term<int>{{1}};

        {
            user_ref<user_term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(std::move(ref))), user_term<double> const &>::value));
        }

        {
            user_ref<user_term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), user_term<double> const &>::value));
        }

        {
            user_ref<user_term<double> const &> const ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), user_term<double> const &>::value));
        }
    }
}
