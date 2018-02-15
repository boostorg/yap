#include <boost/yap/expression.hpp>

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


struct void_callable
{
    void operator()() { *called_ = (*call_count_)++; }
    int * call_count_;
    int * called_;
};

struct int_callable
{
    int operator()()
    {
        *called_ = (*call_count_)++;
        return 42;
    }
    int * call_count_;
    int * called_;
};

struct double_callable
{
    double operator()()
    {
        *called_ = (*call_count_)++;
        return 13.0;
    }
    int * call_count_;
    int * called_;
};


TEST(comma, order_of_eval)
{
    {
        int call_count = 0;
        int int_called = -1;
        int double_called = -1;

        auto int_double_expr =
            (term<int_callable>{{&call_count, &int_called}}(),
             term<double_callable>{{&call_count, &double_called}}());

        EXPECT_EQ(evaluate(int_double_expr), 13.0);
        EXPECT_EQ(int_called, 0);
        EXPECT_EQ(double_called, 1);
    }

    {
        int call_count = 0;
        int int_called = -1;
        int double_called = -1;

        auto double_int_expr =
            (term<double_callable>{{&call_count, &double_called}}(),
             term<int_callable>{{&call_count, &int_called}}());

        EXPECT_EQ(evaluate(double_int_expr), 42);
        EXPECT_EQ(int_called, 1);
        EXPECT_EQ(double_called, 0);
    }
}

TEST(comma, void_expressions)
{
    {
        int call_count = 0;
        int void_called = -1;
        int int_called = -1;

        auto void_int_expr =
            (term<void_callable>{{&call_count, &void_called}}(),
             term<int_callable>{{&call_count, &int_called}}());

        EXPECT_EQ(evaluate(void_int_expr), 42);
        EXPECT_EQ(void_called, 0);
        EXPECT_EQ(int_called, 1);
    }

    {
        int call_count = 0;
        int void_called = -1;
        int int_called = -1;

        auto int_void_expr =
            (term<int_callable>{{&call_count, &int_called}}(),
             term<void_callable>{{&call_count, &void_called}}());

        static_assert(
            std::is_same<void, decltype(evaluate(int_void_expr))>::value);

        evaluate(int_void_expr);
        EXPECT_EQ(void_called, 1);
        EXPECT_EQ(int_called, 0);
    }
}
