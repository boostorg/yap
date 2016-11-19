#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


void compile_placeholders ()
{
    using namespace boost::proto17::literals;

    {
        bp17::placeholder<1> p1 = 1_p;
        (void)p1;
    }

    {
        bp17::placeholder<1> p1 = 1_p;
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<1>,
            term<double>
        > expr = p1 + unity;
        (void)expr;
    }

    {
        bp17::placeholder<1> p1 = 1_p;
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<1>,
            bp17::placeholder<2>
        > expr = p1 + 2_p;
        (void)expr;
    }
}
