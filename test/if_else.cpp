#include <boost/yap/expression.hpp>
#include <boost/yap/expression_if_else.hpp>

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

template <typename T>
using term_ref = boost::yap::expression_ref<boost::yap::expression, term<T> &>;

template <typename T>
using term_cref = boost::yap::expression_ref<boost::yap::expression, term<T> const &>;

namespace yap = boost::yap;
namespace bh = boost::hana;


struct callable
{
    int operator()() { return 42; }
};

struct throwing_callable_1
{
    int operator()()
    {
        throw std::logic_error("Something's wrong.");
        return 0;
    }
};

struct throwing_callable_2
{
    int operator()()
    {
        throw std::length_error("Something's wrong.");
        return 0;
    }
};


TEST(if_else, test)
{
    {
        auto true_nothrow_throw_expr = if_else(
            term<bool>{{true}},
            term<callable>{}(),
            term<throwing_callable_1>{}());

        EXPECT_NO_THROW(yap::evaluate(true_nothrow_throw_expr));
        EXPECT_EQ(yap::evaluate(true_nothrow_throw_expr), 42);
    }

    {
        auto false_nothrow_throw_expr = if_else(
            term<bool>{{false}},
            term<callable>{}(),
            term<throwing_callable_1>{}());

        EXPECT_THROW(yap::evaluate(false_nothrow_throw_expr), std::logic_error);
    }

    {
        auto true_throw1_throw2_expr = if_else(
            term<bool>{{true}},
            term<throwing_callable_1>{}(),
            term<throwing_callable_2>{}());

        EXPECT_THROW(yap::evaluate(true_throw1_throw2_expr), std::logic_error);
    }

    {
        auto false_throw1_throw2_expr = if_else(
            term<bool>{{false}},
            term<throwing_callable_1>{}(),
            term<throwing_callable_2>{}());

        EXPECT_THROW(yap::evaluate(false_throw1_throw2_expr), std::logic_error);
    }
}
