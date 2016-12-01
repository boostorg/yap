#define BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::yap::terminal<T>;

namespace yap = boost::yap;


TEST(left, depth_stress_test)
{
    term<double> unity{1.0};

    {
        auto expr = (unity + (unity + (unity + (unity +    (unity + (unity + (unity + unity)))))));
        double result = expr;
        EXPECT_EQ(result, 8.0);
    }

    {
        auto expr =
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +

            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
        double result = expr;
        EXPECT_EQ(result, 64.0);
    }

    {
        auto expr =
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +

            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
        double result =
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr))))))))))))))))))))))))))))))));
        EXPECT_EQ(result, 64.0 * 32.0);
    }
}
