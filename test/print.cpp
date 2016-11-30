#include "expression.hpp"
#include "print.hpp"

#include <gtest/gtest.h>

#include <sstream>


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

struct thing {};

TEST(expression, test_print)
{
    term<double> unity{1.0};
    int i_ = 42;
    term<int &&> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            bp17::expression_ref<term<double> &>,
            term<int &&>
        >
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            bp17::expression_ref<term<double> &>,
            bp17::expression<
                bp17::expr_kind::plus,
                bh::tuple<
                    bp17::expression_ref<term<double> &>,
                    term<int &&>
                >
            >
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        std::ostringstream oss;
        bp17::print(oss, unity);
        EXPECT_EQ(oss.str(), R"(term<double>[=1]
)");
    }

    {
        std::ostringstream oss;
        bp17::print(oss, expr);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    term<int &&>[=42]
)");
    }

    {
        std::ostringstream oss;
        bp17::print(oss, unevaluated_expr);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    expr<+>
        term<double>[=1] &
        term<int &&>[=42]
)");
    }

    term<thing> a_thing(thing{});

    {
        std::ostringstream oss;
        bp17::print(oss, a_thing);
        EXPECT_EQ(oss.str(), R"(term<thing>[=<<unprintable-value>>]
)");
    }

    term<double> const const_unity{1.0};
    bp17::expression<
        bp17::expr_kind::plus,
        bh::tuple<
            bp17::expression_ref<term<double> &>,
            bp17::expression_ref<term<double> const &>
        >
    > nonconst_plus_const = unity + const_unity;

    {
        std::ostringstream oss;
        bp17::print(oss, nonconst_plus_const);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    term<double>[=1] const &
)");
    }
}

TEST(user_expr, test_print)
{
    user_term<double> unity{1.0};
    int i_ = 42;
    user_term<int &&> i{std::move(i_)};
    user_expr<
        bp17::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_term<int &&>
        >
    > expr = unity + std::move(i);
    user_expr<
        bp17::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_expr<
                bp17::expr_kind::plus,
                bh::tuple<
                    user_ref<user_term<double> &>,
                    user_term<int &&>
                >
            >
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        std::ostringstream oss;
        bp17::print(oss, unity);
        EXPECT_EQ(oss.str(), R"(term<double>[=1]
)");
    }

    {
        std::ostringstream oss;
        bp17::print(oss, expr);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    term<int &&>[=42]
)");
    }

    {
        std::ostringstream oss;
        bp17::print(oss, unevaluated_expr);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    expr<+>
        term<double>[=1] &
        term<int &&>[=42]
)");
    }

    user_term<thing> a_thing{bh::make_tuple(thing{})};

    {
        std::ostringstream oss;
        bp17::print(oss, a_thing);
        EXPECT_EQ(oss.str(), R"(term<thing>[=<<unprintable-value>>]
)");
    }

    user_term<double> const const_unity{1.0};
    user_expr<
        bp17::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_ref<user_term<double> const &>
        >
    > nonconst_plus_const = unity + const_unity;

    {
        std::ostringstream oss;
        bp17::print(oss, nonconst_plus_const);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    term<double>[=1] const &
)");
    }
}
