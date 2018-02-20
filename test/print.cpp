// Copyright (C) 2016-2018 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/yap/expression.hpp>
#include <boost/yap/print.hpp>

#include <gtest/gtest.h>

#include <sstream>


template<typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

template<typename T>
using ref = boost::yap::expression_ref<boost::yap::expression, T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


template<boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    static boost::yap::expr_kind const kind = Kind;

    Tuple elements;

    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(plus, ::user_expr)
};

template<typename T>
using user_term = boost::yap::terminal<user_expr, T>;

template<typename T>
using user_ref = boost::yap::expression_ref<user_expr, T>;

struct thing
{};

TEST(expression, test_print)
{
    term<double> unity{1.0};
    int i_ = 42;
    term<int &&> i{std::move(i_)};
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<ref<term<double> &>, term<int &&>>>
        expr = unity + std::move(i);
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<
            ref<term<double> &>,
            yap::expression<
                yap::expr_kind::plus,
                bh::tuple<ref<term<double> &>, term<int &&>>>>>
        unevaluated_expr = unity + std::move(expr);

    {
        std::ostringstream oss;
        yap::print(oss, unity);
        EXPECT_EQ(oss.str(), R"(term<double>[=1]
)");
    }

    {
        std::ostringstream oss;
        yap::print(oss, expr);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    term<int &&>[=42]
)");
    }

    {
        std::ostringstream oss;
        yap::print(oss, unevaluated_expr);
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
        yap::print(oss, a_thing);
        EXPECT_EQ(oss.str(), R"(term<thing>[=<<unprintable-value>>]
)");
    }

    term<double> const const_unity{1.0};
    yap::expression<
        yap::expr_kind::plus,
        bh::tuple<ref<term<double> &>, ref<term<double> const &>>>
        nonconst_plus_const = unity + const_unity;

    {
        std::ostringstream oss;
        yap::print(oss, nonconst_plus_const);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    term<double>[=1] const &
)");
    }

    {
        using namespace yap::literals;
        std::ostringstream oss;
        yap::print(oss, 1_p);
        EXPECT_EQ(oss.str(), R"(term<boost::yap::placeholder<1ll>>[=1]
)");
    }
}

TEST(user_expr, test_print)
{
    user_term<double> unity{1.0};
    int i_ = 42;
    user_term<int &&> i{std::move(i_)};
    user_expr<
        yap::expr_kind::plus,
        bh::tuple<user_ref<user_term<double> &>, user_term<int &&>>>
        expr = unity + std::move(i);
    user_expr<
        yap::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_expr<
                yap::expr_kind::plus,
                bh::tuple<user_ref<user_term<double> &>, user_term<int &&>>>>>
        unevaluated_expr = unity + std::move(expr);

    {
        std::ostringstream oss;
        yap::print(oss, unity);
        EXPECT_EQ(oss.str(), R"(term<double>[=1]
)");
    }

    {
        std::ostringstream oss;
        yap::print(oss, expr);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    term<int &&>[=42]
)");
    }

    {
        std::ostringstream oss;
        yap::print(oss, unevaluated_expr);
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
        yap::print(oss, a_thing);
        EXPECT_EQ(oss.str(), R"(term<thing>[=<<unprintable-value>>]
)");
    }

    user_term<double> const const_unity{1.0};
    user_expr<
        yap::expr_kind::plus,
        bh::tuple<
            user_ref<user_term<double> &>,
            user_ref<user_term<double> const &>>>
        nonconst_plus_const = unity + const_unity;

    {
        std::ostringstream oss;
        yap::print(oss, nonconst_plus_const);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1] &
    term<double>[=1] const &
)");
    }
}
