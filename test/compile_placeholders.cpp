#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


void compile_placeholders ()
{
    using namespace boost::proto17::literals;

    {
        bp17::placeholder<0> p0 = 0_p;
        (void)p0;
    }

    {
        bp17::placeholder<0> p0 = 0_p;
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<0>,
            term<double>
        > expr = p0 + unity;
        (void)expr;
    }

    {
        bp17::placeholder<0> p0 = 0_p;
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<0>,
            bp17::placeholder<1>
        > expr = p0 + 1_p;
        (void)expr;
    }
}
