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


TEST(expression, test_left)
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
            std::is_same<decltype(yap::left(std::move(plus_expr))),
            ref<term<double> &> &&>::value
        ));
    }

    {
        plus_expr_type plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::left(plus_expr)),
            ref<term<double> &> &>::value
        ));
    }

    {
        plus_expr_type const plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::left(plus_expr)),
            ref<term<double> &> const &>::value
        ));
    }

    {
        term<double> const unity = {{1.0}};
        using plus_expr_type = yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                ref<term<double> const &>,
                term<int>
            >
        >;

        {
            plus_expr_type plus_expr = unity + term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(std::move(plus_expr))),
                ref<term<double> const &> &&>::value
            ));
        }

        {
            plus_expr_type plus_expr = unity + term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr)),
                ref<term<double> const &> &>::value
            ));
        }

        {
            plus_expr_type const plus_expr = unity + term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr)),
                ref<term<double> const &> const &>::value
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
                std::is_same<decltype(yap::left(std::move(plus_expr_ref))),
                ref<term<double> &> &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr_ref)),
                ref<term<double> &> &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr_ref)),
                ref<term<double> &> &>::value
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
                std::is_same<decltype(yap::left(std::move(plus_expr_ref))),
                ref<term<double> &> const &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr_ref)),
                ref<term<double> &> const &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            ref<plus_expr_type const &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr_ref)),
                ref<term<double> &> const &>::value
            ));
        }
    }
}

TEST(user_expr, test_left)
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
            std::is_same<decltype(yap::left(std::move(plus_expr))),
            user_ref<user_term<double> &> &&>::value
        ));
    }

    {
        plus_expr_type plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::left(plus_expr)),
            user_ref<user_term<double> &> &>::value
        ));
    }

    {
        plus_expr_type const plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(yap::left(plus_expr)),
            user_ref<user_term<double> &> const &>::value
        ));
    }

    {
        user_term<double> const unity = {{1.0}};
        using plus_expr_type = user_expr<
            yap::expr_kind::plus,
            bh::tuple<
                user_ref<user_term<double> const &>,
                user_term<int>
            >
        >;

        {
            plus_expr_type plus_expr = unity + user_term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(std::move(plus_expr))),
                user_ref<user_term<double> const &> &&>::value
            ));
        }

        {
            plus_expr_type plus_expr = unity + user_term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr)),
                user_ref<user_term<double> const &> &>::value
            ));
        }

        {
            plus_expr_type const plus_expr = unity + user_term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr)),
                user_ref<user_term<double> const &> const &>::value
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
                std::is_same<decltype(yap::left(std::move(plus_expr_ref))),
                user_ref<user_term<double> &> &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr_ref)),
                user_ref<user_term<double> &> &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr_ref)),
                user_ref<user_term<double> &> &>::value
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
                std::is_same<decltype(yap::left(std::move(plus_expr_ref))),
                user_ref<user_term<double> &> const &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr_ref)),
                user_ref<user_term<double> &> const &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type const &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(yap::left(plus_expr_ref)),
                user_ref<user_term<double> &> const &>::value
            ));
        }
    }
}
