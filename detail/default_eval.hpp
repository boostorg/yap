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
            }

#define BOOST_PROTO17_UNARY_OPERATOR_CASE(op_name)                      \
            else if constexpr (Kind == expr_kind:: op_name) {           \
                return                                                  \
                    eval_## op_name(                                    \
                        default_eval_expr(expr.elements[0_c], static_cast<Tuple &&>(args)) \
                    );                                                  \
            }

            BOOST_PROTO17_UNARY_OPERATOR_CASE(unary_plus) // +
            BOOST_PROTO17_UNARY_OPERATOR_CASE(negate) // -
            BOOST_PROTO17_UNARY_OPERATOR_CASE(dereference) // *
            BOOST_PROTO17_UNARY_OPERATOR_CASE(complement) // ~
            BOOST_PROTO17_UNARY_OPERATOR_CASE(address_of) // &
            BOOST_PROTO17_UNARY_OPERATOR_CASE(logical_not) // !
            BOOST_PROTO17_UNARY_OPERATOR_CASE(pre_inc) // ++
            BOOST_PROTO17_UNARY_OPERATOR_CASE(pre_dec) // --
            BOOST_PROTO17_UNARY_OPERATOR_CASE(post_inc) // ++(int)
            BOOST_PROTO17_UNARY_OPERATOR_CASE(post_dec) // --(int)

#define BOOST_PROTO17_UNARY_OPERATOR_CASE

#define BOOST_PROTO17_BINARY_OPERATOR_CASE(op_name)                     \
            else if constexpr (Kind == expr_kind:: op_name) {           \
                return                                                  \
                    eval_## op_name(                                    \
                        default_eval_expr(expr.elements[0_c], static_cast<Tuple &&>(args)), \
                        default_eval_expr(expr.elements[1_c], static_cast<Tuple &&>(args)) \
                    );                                                  \
            }

            BOOST_PROTO17_BINARY_OPERATOR_CASE(shift_left) // <<
            BOOST_PROTO17_BINARY_OPERATOR_CASE(shift_right) // >>
            BOOST_PROTO17_BINARY_OPERATOR_CASE(multiplies) // *
            BOOST_PROTO17_BINARY_OPERATOR_CASE(divides) // /
            BOOST_PROTO17_BINARY_OPERATOR_CASE(modulus) // %
            BOOST_PROTO17_BINARY_OPERATOR_CASE(plus) // +
            BOOST_PROTO17_BINARY_OPERATOR_CASE(minus) // -
            BOOST_PROTO17_BINARY_OPERATOR_CASE(less) // <
            BOOST_PROTO17_BINARY_OPERATOR_CASE(greater) // >
            BOOST_PROTO17_BINARY_OPERATOR_CASE(less_equal) // <=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(greater_equal) // >=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(equal_to) // ==
            BOOST_PROTO17_BINARY_OPERATOR_CASE(not_equal_to) // !=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(logical_or) // ||
            BOOST_PROTO17_BINARY_OPERATOR_CASE(logical_and) // &&
            BOOST_PROTO17_BINARY_OPERATOR_CASE(bitwise_and) // &
            BOOST_PROTO17_BINARY_OPERATOR_CASE(bitwise_or) // |
            BOOST_PROTO17_BINARY_OPERATOR_CASE(bitwise_xor) // ^

            else if constexpr (Kind == expr_kind::comma) {
                return
                    eval_comma(
                        default_eval_expr(expr.elements[0_c], static_cast<Tuple &&>(args)),
                        default_eval_expr(expr.elements[1_c], static_cast<Tuple &&>(args))
                    );
            }

            BOOST_PROTO17_BINARY_OPERATOR_CASE(mem_ptr) // ->*
            BOOST_PROTO17_BINARY_OPERATOR_CASE(assign) // =
            BOOST_PROTO17_BINARY_OPERATOR_CASE(shift_left_assign) // <<=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(shift_right_assign) // >>=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(multiplies_assign) // *=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(divides_assign) // /=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(modulus_assign) // %=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(plus_assign) // +=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(minus_assign) // -=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(bitwise_and_assign) // &=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(bitwise_or_assign) // |=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(bitwise_xor_assign) // ^=
            BOOST_PROTO17_BINARY_OPERATOR_CASE(bitwise_xor_assign) // []

#undef BOOST_PROTO17_BINARY_OPERATOR_CASE

            else if constexpr (Kind == expr_kind::call) {
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
