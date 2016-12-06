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
    using this_type = user_expr<Kind, Tuple>;

    static boost::yap::expr_kind const kind = Kind;

    Tuple elements;
    
    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(plus, this_type, ::user_expr)
};

template <typename T>
using user_term = boost::yap::terminal<user_expr, T>;

template <typename T>
using user_ref = boost::yap::expression_ref<user_expr, T>;


TEST(expression, test_right)
{
    term<double> unity = {{1.0}};
    using plus_expr_type = yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            ref<term<double> &>,
            term<int>
        >
    >;

    {
        plus_expr_type plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::right(std::move(plus_expr))),
            term<int> &&>::value
        ));
    }

    {
        plus_expr_type plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::right(plus_expr)),
            term<int> &>::value
        ));
    }

    {
        plus_expr_type const plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::right(plus_expr)),
            term<int> const &>::value
        ));
    }

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

        using plus_plus_expr_type = yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                ref<plus_expr_type &>,
                term<int>
            >
        >;

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(std::move(plus_expr_ref))),
                term<int> &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(plus_expr_ref)),
                term<int> &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(plus_expr_ref)),
                term<int> &>::value
            ));
        }
    }

    {
        term<double> unity = {{1.0}};
        using plus_expr_type = yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                ref<term<double> &>,
                term<int>
            >
        >;
        plus_expr_type const plus_expr = unity + term<int>{{1}};

        using plus_plus_expr_type = yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                ref<plus_expr_type const &>,
                term<int>
            >
        >;

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(std::move(plus_expr_ref))),
                term<int> const &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(plus_expr_ref)),
                term<int> const &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type const &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(plus_expr_ref)),
                term<int> const &>::value
            ));
        }
    }
}

TEST(user_expr, test_right)
{
    user_term<double> unity = {{1.0}};
    using plus_expr_type = user_expr<
        yap::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_term<int>
        >
    >;

    {
        plus_expr_type plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::right(std::move(plus_expr))),
            user_term<int> &&>::value
        ));
    }

    {
        plus_expr_type plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::right(plus_expr)),
            user_term<int> &>::value
        ));
    }

    {
        plus_expr_type const plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::right(plus_expr)),
            user_term<int> const &>::value
        ));
    }

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

        using plus_plus_expr_type = user_expr<
            yap::expr_kind::plus,
            bh::tuple<
                user_ref<plus_expr_type &>,
                user_term<int>
            >
        >;

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(std::move(plus_expr_ref))),
                user_term<int> &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(plus_expr_ref)),
                user_term<int> &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(plus_expr_ref)),
                user_term<int> &>::value
            ));
        }
    }

    {
        user_term<double> unity = {{1.0}};
        using plus_expr_type = user_expr<
            yap::expr_kind::plus,
            bh::tuple<
                user_ref<user_term<double> &>,
                user_term<int>
            >
        >;
        plus_expr_type const plus_expr = unity + user_term<int>{{1}};

        using plus_plus_expr_type = user_expr<
            yap::expr_kind::plus,
            bh::tuple<
                user_ref<plus_expr_type const &>,
                user_term<int>
            >
        >;

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(std::move(plus_expr_ref))),
                user_term<int> const &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(plus_expr_ref)),
                user_term<int> const &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type const &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::right(plus_expr_ref)),
                user_term<int> const &>::value
            ));
        }
    }
}
