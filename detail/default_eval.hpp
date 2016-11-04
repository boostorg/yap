#ifndef BOOST_PROTO17_DETAIL_DEFAULT_EVAL_HPP_INCLUDED
#define BOOST_PROTO17_DETAIL_DEFAULT_EVAL_HPP_INCLUDED

#include "../expression_fwd.hpp"
#include "../operators.hpp"

#include <cassert>


namespace boost::proto17 {

    namespace detail {

        template <typename Tuple, expr_kind Kind, typename ...T>
        auto default_eval_expr (expression<Kind, T...> const & expr, Tuple && tuple)
        {
            using namespace hana::literals;
            if constexpr (Kind == expr_kind::terminal) {
                static_assert(sizeof...(T) == 1);
                return expr.elements[0_c];
            } else if constexpr (Kind == expr_kind::placeholder) {
                static_assert(sizeof...(T) == 1);
                return tuple[expr.elements[0_c]];
            } else if constexpr (Kind == expr_kind::plus) {
                return
                    eval_plus(
                        default_eval_expr(expr.elements[0_c], static_cast<Tuple &&>(tuple)),
                        default_eval_expr(expr.elements[1_c], static_cast<Tuple &&>(tuple))
                    );
            } else if constexpr (Kind == expr_kind::minus) {
                return
                    eval_minus(
                        default_eval_expr(expr.elements[0_c], static_cast<Tuple &&>(tuple)),
                        default_eval_expr(expr.elements[1_c], static_cast<Tuple &&>(tuple))
                    );
            } else {
                assert(false && "Unhandled expr_kind in default_evaluate!");
                return;
            }
        }

    }

}

#endif
