#include "expression.hpp"

#include <gtest/gtest.h>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
namespace bh = boost::hana;


template <boost::proto17::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static boost::proto17::expr_kind const kind = Kind;

    Tuple elements;
    
    BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(plus, this_type, ::user_expr)
};

template <typename T>
using user_term = boost::proto17::terminal<T, user_expr>;

template <typename T>
using user_ref = boost::proto17::expression_ref<T, user_expr>;


TEST(expression, test_left)
{
    term<double> unity = {{1.0}};
    using plus_expr_type = bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            bp17::expression_ref<term<double> &>,
            term<int>
        >
    >;

    {
        plus_expr_type plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(bp17::left(std::move(plus_expr))),
            bp17::expression_ref<term<double> &> &&>::value
        ));
    }

    {
        plus_expr_type plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(bp17::left(plus_expr)),
            bp17::expression_ref<term<double> &> &>::value
        ));
    }

    {
        plus_expr_type const plus_expr = unity + term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(bp17::left(plus_expr)),
            bp17::expression_ref<term<double> &> const &>::value
        ));
    }

    {
        term<double> const unity = {{1.0}};
        using plus_expr_type = bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> const &>,
                term<int>
            >
        >;

        {
            plus_expr_type plus_expr = unity + term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(std::move(plus_expr))),
                bp17::expression_ref<term<double> const &> &&>::value
            ));
        }

        {
            plus_expr_type plus_expr = unity + term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr)),
                bp17::expression_ref<term<double> const &> &>::value
            ));
        }

        {
            plus_expr_type const plus_expr = unity + term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr)),
                bp17::expression_ref<term<double> const &> const &>::value
            ));
        }
    }

    {
        term<double> unity = {{1.0}};
        using plus_expr_type = bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int>
            >
        >;
        plus_expr_type plus_expr = unity + term<int>{{1}};

        using plus_plus_expr_type = bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<plus_expr_type &>,
                term<int>
            >
        >;

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            bp17::expression_ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(std::move(plus_expr_ref))),
                bp17::expression_ref<term<double> &> &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            bp17::expression_ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr_ref)),
                bp17::expression_ref<term<double> &> &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            bp17::expression_ref<plus_expr_type &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr_ref)),
                bp17::expression_ref<term<double> &> &>::value
            ));
        }
    }

    {
        term<double> unity = {{1.0}};
        using plus_expr_type = bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int>
            >
        >;
        plus_expr_type const plus_expr = unity + term<int>{{1}};

        using plus_plus_expr_type = bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<plus_expr_type const &>,
                term<int>
            >
        >;

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            bp17::expression_ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(std::move(plus_expr_ref))),
                bp17::expression_ref<term<double> &> const &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            bp17::expression_ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr_ref)),
                bp17::expression_ref<term<double> &> const &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + term<int>{{1}};
            bp17::expression_ref<plus_expr_type const &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr_ref)),
                bp17::expression_ref<term<double> &> const &>::value
            ));
        }
    }
}

TEST(user_expr, test_left)
{
    user_term<double> unity = {{1.0}};
    using plus_expr_type = user_expr<
        bp17::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_term<int>
        >
    >;

    {
        plus_expr_type plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(bp17::left(std::move(plus_expr))),
            user_ref<user_term<double> &> &&>::value
        ));
    }

    {
        plus_expr_type plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(bp17::left(plus_expr)),
            user_ref<user_term<double> &> &>::value
        ));
    }

    {
        plus_expr_type const plus_expr = unity + user_term<int>{{1}};
        EXPECT_TRUE((
            std::is_same<decltype(bp17::left(plus_expr)),
            user_ref<user_term<double> &> const &>::value
        ));
    }

    {
        user_term<double> const unity = {{1.0}};
        using plus_expr_type = user_expr<
            bp17::expr_kind::plus,
            bh::tuple<
                user_ref<user_term<double> const &>,
                user_term<int>
            >
        >;

        {
            plus_expr_type plus_expr = unity + user_term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(std::move(plus_expr))),
                user_ref<user_term<double> const &> &&>::value
            ));
        }

        {
            plus_expr_type plus_expr = unity + user_term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr)),
                user_ref<user_term<double> const &> &>::value
            ));
        }

        {
            plus_expr_type const plus_expr = unity + user_term<int>{{1}};
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr)),
                user_ref<user_term<double> const &> const &>::value
            ));
        }
    }

    {
        user_term<double> unity = {{1.0}};
        using plus_expr_type = user_expr<
            bp17::expr_kind::plus,
            bh::tuple<
                user_ref<user_term<double> &>,
                user_term<int>
            >
        >;
        plus_expr_type plus_expr = unity + user_term<int>{{1}};

        using plus_plus_expr_type = user_expr<
            bp17::expr_kind::plus,
            bh::tuple<
                user_ref<plus_expr_type &>,
                user_term<int>
            >
        >;

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(std::move(plus_expr_ref))),
                user_ref<user_term<double> &> &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr_ref)),
                user_ref<user_term<double> &> &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr_ref)),
                user_ref<user_term<double> &> &>::value
            ));
        }
    }

    {
        user_term<double> unity = {{1.0}};
        using plus_expr_type = user_expr<
            bp17::expr_kind::plus,
            bh::tuple<
                user_ref<user_term<double> &>,
                user_term<int>
            >
        >;
        plus_expr_type const plus_expr = unity + user_term<int>{{1}};

        using plus_plus_expr_type = user_expr<
            bp17::expr_kind::plus,
            bh::tuple<
                user_ref<plus_expr_type const &>,
                user_term<int>
            >
        >;

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(std::move(plus_expr_ref))),
                user_ref<user_term<double> &> const &>::value
            ));

        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type const &> plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr_ref)),
                user_ref<user_term<double> &> const &>::value
            ));
        }

        {
            plus_plus_expr_type plus_plus_expr = plus_expr + user_term<int>{{1}};
            user_ref<plus_expr_type const &> const plus_expr_ref = bh::front(plus_plus_expr.elements);
            EXPECT_TRUE((
                std::is_same<decltype(bp17::left(plus_expr_ref)),
                user_ref<user_term<double> &> const &>::value
            ));
        }
    }
}
