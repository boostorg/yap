#include <boost/yap/expression.hpp>

#include <gtest/gtest.h>


template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

namespace yap = boost::yap;

namespace reference_returning {

    struct number
    {
        double value;
    };

    number a_result{3.0};
    number const the_result{13.0};

    number const & operator+ (number a, number b)
    { return the_result; }

    number & operator- (number a, number b)
    { return a_result; }

}

TEST(reference_returns, test_reference_returns)
{
    term<reference_returning::number> unity = {{1.0}};
    auto plus_expr = unity + reference_returning::number{1.0};

    {
        reference_returning::number const & n = evaluate(plus_expr);
        EXPECT_EQ(&n, &reference_returning::the_result);
    }

    static_assert(
        std::is_same<
            decltype(evaluate(plus_expr)),
            reference_returning::number const &
        >{}, "type failure"
    );

    auto minus_expr = unity - reference_returning::number{1.0};

    {
        reference_returning::number & n = evaluate(minus_expr);
        EXPECT_EQ(&n, &reference_returning::a_result);
    }

    static_assert(
        std::is_same<
            decltype(evaluate(minus_expr)),
            reference_returning::number &
        >{}, "type failure"
    );

    using namespace yap::literals;

    {
        reference_returning::number & n = evaluate(1_p, reference_returning::a_result);
        EXPECT_EQ(&n, &reference_returning::a_result);
    }

    static_assert(
        std::is_same<
            decltype(evaluate(1_p, reference_returning::a_result)),
            reference_returning::number &
        >{}, "type failure"
    );

    {
        reference_returning::number const & n = evaluate(1_p, reference_returning::the_result);
        EXPECT_EQ(&n, &reference_returning::the_result);
    }

    static_assert(
        std::is_same<
            decltype(evaluate(1_p, reference_returning::the_result)),
            reference_returning::number const &
        >{}, "type failure"
    );
}
