#include <boost/yap/expression.hpp>

#include <memory>


template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

namespace yap = boost::yap;


inline auto double_to_float (term<double> expr)
{ return term<float>{(float)expr.value()}; }

struct copy_only_t
{
    copy_only_t (copy_only_t &&) = delete;
    copy_only_t & operator= (copy_only_t &&) = delete;

    int value;
};

void compile_copy_only_types ()
{
#if 0 // TODO: This ICEs!
    term<copy_only_t> unity{1};
    term<double> d{3.5};
    yap::expression<
        yap::expr_kind::plus,
        term<copy_only_t>,
        term<double>
    > expr_1 = unity + d;

    yap::expression<
        yap::expr_kind::plus,
        term<copy_only_t>,
        yap::expression<
            yap::expr_kind::plus,
            term<copy_only_t>,
            term<double>
        >
    > expr_2 = unity + expr_1;

    auto transformed_expr = transform(expr_2, double_to_float);
    (void)transformed_expr;
#endif
}
