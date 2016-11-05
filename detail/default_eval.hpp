#ifndef BOOST_PROTO17_DETAIL_DEFAULT_EVAL_HPP_INCLUDED
#define BOOST_PROTO17_DETAIL_DEFAULT_EVAL_HPP_INCLUDED

#include "../expression_fwd.hpp"
#include "../operators.hpp"

#include <boost/hana/transform.hpp>

#include <cassert>


namespace boost::proto17 {

    namespace detail {

        template <typename Tuple, expr_kind Kind, typename ...T>
        decltype(auto) default_eval_expr (expression<Kind, T...> const & expr, Tuple && args)
        {
            using namespace hana::literals;
            if constexpr (Kind == expr_kind::terminal) {
                static_assert(sizeof...(T) == 1);
                return expr.elements[0_c];
            } else if constexpr (Kind == expr_kind::placeholder) {
                static_assert(sizeof...(T) == 1);
                return args[expr.elements[0_c]];
            } else if constexpr (Kind == expr_kind::plus) {
                return
                    eval_plus(
                        default_eval_expr(expr.elements[0_c], static_cast<Tuple &&>(args)),
                        default_eval_expr(expr.elements[1_c], static_cast<Tuple &&>(args))
                    );
            } else if constexpr (Kind == expr_kind::minus) {
                return
                    eval_minus(
                        default_eval_expr(expr.elements[0_c], static_cast<Tuple &&>(args)),
                        default_eval_expr(expr.elements[1_c], static_cast<Tuple &&>(args))
                    );
            } else if constexpr (Kind == expr_kind::call) {
                return hana::unpack(
                    hana::transform(expr.elements, [&args] (auto && element) {
                        return default_eval_expr(element, static_cast<Tuple &&>(args));
                    }),
                    eval_call
                );
            } else {
                assert(false && "Unhandled expr_kind in default_evaluate!");
                return;
            }
        }

    }

}

#endif
