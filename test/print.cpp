#include "expression.hpp"

#define BOOST_PROTO17_STREAM_OPERATORS
#include "print.hpp"

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


TEST(print, test_print)
{
    term<double> unity{1.0};
    int i_ = 42;
    term<int &&> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        term<int &&>
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
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
    term<double>[=1]
    term<int &&>[=42]
)");
    }

    {
        std::ostringstream oss;
        bp17::print(oss, unevaluated_expr);
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1]
    expr<+>
        term<double>[=1]
        term<int &&>[=42]
)");
    }

    struct thing {};
    term<thing> a_thing(thing{});

    {
        std::ostringstream oss;
        bp17::print(oss, a_thing);
        EXPECT_EQ(oss.str(), R"(term<print_test_print_Test::TestBody()::thing>[=<<unprintable-value>>]
)");
    }

    {
        using namespace boost::proto17::literals;

        {
            std::ostringstream oss;
            oss << (0_p + unity);
            EXPECT_EQ(oss.str(), R"(expr<+>
    placeholder<0>
    term<double>[=1]
)");
        }

        {
            std::ostringstream oss;
            oss << (2_p + 3_p);
            EXPECT_EQ(oss.str(), R"(expr<+>
    placeholder<2>
    placeholder<3>
)");
        }

        {
            std::ostringstream oss;
            oss << (unity + 1_p);
            EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1]
    placeholder<1>
)");
        }
    }

    {
        std::ostringstream oss;
        oss << unity;
        EXPECT_EQ(oss.str(), R"(term<double>[=1]
)");
    }

    {
        std::ostringstream oss;
        oss << expr;
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1]
    term<int &&>[=42]
)");
    }

    {
        std::ostringstream oss;
        oss << unevaluated_expr;
        EXPECT_EQ(oss.str(), R"(expr<+>
    term<double>[=1]
    expr<+>
        term<double>[=1]
        term<int &&>[=42]
)");
    }

    {
        std::ostringstream oss;
        oss << a_thing;
        EXPECT_EQ(oss.str(), R"(term<print_test_print_Test::TestBody()::thing>[=<<unprintable-value>>]
)");
    }
}
