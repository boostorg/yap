#include "expression.hpp"

#include <memory>

template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


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
    bp17::expression<
        bp17::expr_kind::plus,
        term<copy_only_t>,
        term<double>
    > expr_1 = unity + d;

    bp17::expression<
        bp17::expr_kind::plus,
        term<copy_only_t>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<copy_only_t>,
            term<double>
        >
    > expr_2 = unity + expr_1;

    auto transformed_expr = transform(expr_2, double_to_float);
    (void)transformed_expr;
#endif
}
