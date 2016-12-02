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


TEST(builtin, test_value)
{
    {
        EXPECT_TRUE((std::is_same<decltype(yap::value(1.0)), double &&>::value));
        EXPECT_EQ(yap::value(1.0), 1.0);
    }

    {
        double d = 2.0;
        EXPECT_TRUE((std::is_same<decltype(yap::value(d)), double &>::value));
        EXPECT_EQ(yap::value(d), 2.0);
    }

    {
        double const d = 3.0;
        EXPECT_TRUE((std::is_same<decltype(yap::value(d)), double const &>::value));
        EXPECT_EQ(yap::value(d), 3.0);
    }
}

TEST(expression, test_value)
{
    {
        term<double> td = {{1.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::value(std::move(td))), double &&>::value));
        EXPECT_EQ(yap::value(std::move(td)), 1.0);
    }

    {
        term<double> td = {{2.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::value(td)), double &>::value));
        EXPECT_EQ(yap::value(td), 2.0);
    }

    {
        term<double> const td = {{3.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::value(td)), double const &>::value));
        EXPECT_EQ(yap::value(td), 3.0);
    }

    term<double> unity = {{1.0}};
    using plus_expr_type = yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            yap::expression_ref<term<double> &>,
            term<int>
        >
    >;
    plus_expr_type plus_expr = unity + term<int>{{1}};

    {
        yap::expression_ref<term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::value(std::move(ref))), double &>::value));
    }

    {
        yap::expression_ref<term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::value(ref)), double &>::value));
    }

    {
        yap::expression_ref<term<double> &> const ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::value(ref)), double &>::value));
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
            EXPECT_TRUE((std::is_same<decltype(yap::value(std::move(ref))), double const &>::value));
        }

        {
            yap::expression_ref<term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::value(ref)), double const &>::value));
        }

        {
            yap::expression_ref<term<double> const &> const ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::value(ref)), double const &>::value));
        }
    }
}

TEST(user_expr, test_value)
{
    {
        user_term<double> td = {{1.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::value(std::move(td))), double &&>::value));
        EXPECT_EQ(yap::value(std::move(td)), 1.0);
    }

    {
        user_term<double> td = {{2.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::value(td)), double &>::value));
        EXPECT_EQ(yap::value(td), 2.0);
    }

    {
        user_term<double> const td = {{3.0}};
        EXPECT_TRUE((std::is_same<decltype(yap::value(td)), double const &>::value));
        EXPECT_EQ(yap::value(td), 3.0);
    }

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
        EXPECT_TRUE((std::is_same<decltype(yap::value(std::move(ref))), double &>::value));
    }

    {
        user_ref<user_term<double> &> ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::value(ref)), double &>::value));
    }

    {
        user_ref<user_term<double> &> const ref = bh::front(plus_expr.elements);
        EXPECT_TRUE((std::is_same<decltype(yap::value(ref)), double &>::value));
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
            EXPECT_TRUE((std::is_same<decltype(yap::value(std::move(ref))), double const &>::value));
        }

        {
            user_ref<user_term<double> const &> ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::value(ref)), double const &>::value));
        }

        {
            user_ref<user_term<double> const &> const ref = bh::front(plus_expr.elements);
            EXPECT_TRUE((std::is_same<decltype(yap::value(ref)), double const &>::value));
        }
    }
}
