// Copyright (C) 2016-2018 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/yap/expression.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <sstream>


template<typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

template<typename T>
using term_ref = boost::yap::expression_ref<boost::yap::expression, term<T> &>;

template<typename T>
using term_cref =
    boost::yap::expression_ref<boost::yap::expression, term<T> const &>;

namespace yap = boost::yap;
namespace bh = boost::hana;


struct callable
{
    int operator()() { return 42; }
};

struct exception_1
{};
struct exception_2
{};

struct throwing_callable_1
{
    int operator()()
    {
        throw exception_1{};
        return 0;
    }
};

struct throwing_callable_2
{
    int operator()()
    {
        throw exception_2{};
        return 0;
    }
};


BOOST_AUTO_TEST_CASE(test_if_else)
{
    {
        auto true_nothrow_throw_expr = if_else(
            term<bool>{{true}},
            term<callable>{}(),
            term<throwing_callable_1>{}());

        BOOST_CHECK_NO_THROW(yap::evaluate(true_nothrow_throw_expr));
        BOOST_CHECK(yap::evaluate(true_nothrow_throw_expr) == 42);
    }

    {
        auto false_nothrow_throw_expr = if_else(
            term<bool>{{false}},
            term<callable>{}(),
            term<throwing_callable_1>{}());

        BOOST_CHECK_THROW(yap::evaluate(false_nothrow_throw_expr), exception_1);
    }

    {
        auto true_throw1_throw2_expr = if_else(
            term<bool>{{true}},
            term<throwing_callable_1>{}(),
            term<throwing_callable_2>{}());

        BOOST_CHECK_THROW(yap::evaluate(true_throw1_throw2_expr), exception_1);
    }

    {
        auto false_throw1_throw2_expr = if_else(
            term<bool>{{false}},
            term<throwing_callable_1>{}(),
            term<throwing_callable_2>{}());

        BOOST_CHECK_THROW(yap::evaluate(false_throw1_throw2_expr), exception_2);
    }
}
