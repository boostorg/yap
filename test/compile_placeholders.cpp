#include <boost/yap/expression.hpp>


template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

template <typename T>
using ref = boost::yap::expression_ref<boost::yap::expression, T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


void compile_placeholders ()
{
    using namespace boost::yap::literals;

    {
        yap::placeholder<1> p1 = 1_p;
        (void)p1;
    }

    {
        yap::placeholder<1> p1 = 1_p;
        term<double> unity{1.0};
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                ref<yap::placeholder<1> &>,
                ref<term<double> &>
            >
        > expr = p1 + unity;
        (void)expr;
    }

    {
        yap::placeholder<1> p1 = 1_p;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                ref<yap::placeholder<1> &>,
                yap::placeholder<2>
            >
        > expr = p1 + 2_p;
        (void)expr;
    }
}
