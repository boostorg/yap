#include <boost/yap/expression.hpp>

#include <gtest/gtest.h>


template <typename T>
using term = boost::yap::terminal<T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static boost::yap::expr_kind const kind = Kind;

    Tuple elements;
    
    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(plus, this_type, ::user_expr)
};

template <typename T>
using user_term = boost::yap::terminal<T, user_expr>;

template <typename T>
using user_ref = boost::yap::expression_ref<T, user_expr>;


TEST(builtin, test_deref)
{
    {
        EXPECT_TRUE((std::is_same<decltype(yap::deref(1.0)), double &&>::value));
        EXPECT_EQ(yap::deref(1.0), 1.0);
    }

    {
        double d = 2.0;
        EXPECT_TRUE((std::is_same<decltype(yap::deref(d)), double &>::value));
        EXPECT_EQ(yap::deref(d), 2.0);
    }

    {
        double const d = 3.0;
        EXPECT_TRUE((std::is_same<decltype(yap::deref(d)), double const &>::value));
        EXPECT_EQ(yap::deref(d), 3.0);
    }
}

TEST(expression, test_deref)
{
    {
        EXPECT_TRUE((std::is_same<decltype(yap::deref(term<double>{{1.0}})), term<double> &&>::value));
        EXPECT_EQ(bh::front(yap::deref(term<double>{{1.0}}).elements), 1.0);
    }

    {
        term<double> td = {{2.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::deref(td)), term<double> &>::value));
        EXPECT_EQ(bh::front(yap::deref(td).elements), 2.0);
    }

    {
        term<double> const td = {{3.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::deref(td)), term<double> const &>::value));
        EXPECT_EQ(bh::front(yap::deref(td).elements), 3.0);
    }

    term<double> unity = {{1.0}};
    using plus_expr_type = yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            yap::expression_ref<term<double> &>,
            term<int>
        >
    >;

    {
        EXPECT_TRUE((std::is_same<decltype(yap::deref(unity + term<int>{{1}})), plus_expr_type &&>::value));
    }

    {
        plus_expr_type plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((std::is_same<decltype(yap::deref(plus_expr)), plus_expr_type &>::value));
    }

    {
        plus_expr_type const plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((std::is_same<decltype(yap::deref(plus_expr)), plus_expr_type const &>::value));
    }

    plus_expr_type plus_expr = unity + term<int>{{1}};

    {
        yap::expression_ref<term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(std::move(ref))), double &>::value));
    }

    {
        yap::expression_ref<term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), double &>::value));
    }

    {
        yap::expression_ref<term<double> &> const ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), double &>::value));
    }

    {
        term<double> const unity = {{1.0}};
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                yap::expression_ref<term<double> const &>,
                term<int>
            >
        > plus_expr = unity + term<int>{{1}};

        {
            yap::expression_ref<term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(std::move(ref))), double const &>::value));
        }

        {
            yap::expression_ref<term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), double const &>::value));
        }

        {
            yap::expression_ref<term<double> const &> const ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), double const &>::value));
        }
    }
}

TEST(user_expr, test_deref)
{
    {
        EXPECT_TRUE((std::is_same<decltype(yap::deref(user_term<double>{{1.0}})), user_term<double> &&>::value));
        EXPECT_EQ(bh::front(yap::deref(user_term<double>{{1.0}}).elements), 1.0);
    }

    {
        user_term<double> td = {{2.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::deref(td)), user_term<double> &>::value));
        EXPECT_EQ(bh::front(yap::deref(td).elements), 2.0);
    }

    {
        user_term<double> const td = {{3.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::deref(td)), user_term<double> const &>::value));
        EXPECT_EQ(bh::front(yap::deref(td).elements), 3.0);
    }

    user_term<double> unity = {{1.0}};
    using plus_expr_type = user_expr<
        yap::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_term<int>
        >
    >;

    {
        EXPECT_TRUE((std::is_same<decltype(yap::deref(unity + user_term<int>{{1}})), plus_expr_type &&>::value));
    }

    {
        plus_expr_type plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((std::is_same<decltype(yap::deref(plus_expr)), plus_expr_type &>::value));
    }

    {
        plus_expr_type const plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((std::is_same<decltype(yap::deref(plus_expr)), plus_expr_type const &>::value));
    }

    plus_expr_type plus_expr = unity + user_term<int>{{1}};

    {
        user_ref<user_term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(std::move(ref))), double &>::value));
    }

    {
        user_ref<user_term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), double &>::value));
    }

    {
        user_ref<user_term<double> &> const ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), double &>::value));
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
            EXPECT_TRUE((std::is_same<decltype(yap::deref(std::move(ref))), double const &>::value));
        }

        {
            user_ref<user_term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), double const &>::value));
        }

        {
            user_ref<user_term<double> const &> const ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::deref(ref)), double const &>::value));
        }
    }
}
