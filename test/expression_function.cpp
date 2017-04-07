#include <boost/yap/expression.hpp>

#include <gtest/gtest.h>


template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

template <typename T>
using ref = boost::yap::expression_ref<boost::yap::expression, T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


TEST(expression_function, test_lvalue)
{
    term<int> number = {{42}};

    auto fn = yap::make_expression_function(number);
    auto fn_copy = fn;

    EXPECT_EQ(fn(), 42);
    EXPECT_EQ(fn_copy(), 42);

    yap::value(number) = 21;

    EXPECT_EQ(fn(), 21);
    EXPECT_EQ(fn_copy(), 21);
}

TEST(expression_function, test_rvalue)
{
    term<int> number = {{42}};

    auto fn = yap::make_expression_function(std::move(number));
    auto fn_copy = fn;

    EXPECT_EQ(fn(), 42);
    EXPECT_EQ(fn_copy(), 42);

    yap::value(number) = 21;

    EXPECT_EQ(fn(), 42);
    EXPECT_EQ(fn_copy(), 42);
}

TEST(expression_function, test_move_only_rvalue)
{
    term<std::unique_ptr<int>> number = {{std::unique_ptr<int>(new int(42))}};

    auto fn = yap::make_expression_function(std::move(number));

    EXPECT_EQ(*fn(), 42);

    auto fn_2 = std::move(fn);
    EXPECT_EQ(*fn_2(), 42);

    yap::value(number) = std::unique_ptr<int>(new int(21));

    EXPECT_EQ(*fn_2(), 42);
}
