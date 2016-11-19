#include "expression.hpp"

#include <memory>

template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


inline auto double_to_float (term<double> expr)
{ return term<float>{(float)expr.value()}; }

void compile_move_only_types ()
{
    term<double> unity{1.0};
    term<std::unique_ptr<int>> i{new int{7}};
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        term<std::unique_ptr<int>>
    > expr_1 = unity + std::move(i);

    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<std::unique_ptr<int>>
        >
    > expr_2 = unity + std::move(expr_1);

    auto transformed_expr = transform(std::move(expr_2), double_to_float);
    (void)transformed_expr;
}
