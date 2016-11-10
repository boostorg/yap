#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;

namespace reference_returning {

    struct number
    {
        double value;
    };

    number const the_result{13.0};

    // User-defined binary-plus!  With weird reference semantics!
    template <typename A, typename B>
    inline number const & eval_plus (A a, B b)
    { return the_result; }

}

TEST(reference_returns, test_reference_returns)
{
    term<reference_returning::number> unity = {{1.0}};
    auto expr = unity + reference_returning::number{1.0};

    // TODO: This has to fail due to the general implicit declaration rules
    // (see [conv]/3-6).  This needs to be noted in docs.
#if 0
    {
        reference_returning::number const & n = expr;
        EXPECT_EQ(&n, &reference_returning::the_result);
    }
#endif

    {
        reference_returning::number const & n = evaluate(expr);
        EXPECT_EQ(&n, &reference_returning::the_result);
    }

    static_assert(
        std::is_same_v<
            decltype(evaluate(expr)),
            reference_returning::number const &
        >
    );
}
