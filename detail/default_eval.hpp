#ifndef BOOST_PROTO17_DETAIL_DEFAULT_EVAL_HPP_INCLUDED
#define BOOST_PROTO17_DETAIL_DEFAULT_EVAL_HPP_INCLUDED

#include "../expression_fwd.hpp"
#include "../operators.hpp"

#include <boost/hana/transform.hpp>

#include <cassert>


namespace boost::proto17 {

    namespace detail {

        struct nonexistent_transform {};
        inline nonexistent_transform transform_expression (...) { return {}; }

        template <typename I, typename T>
        decltype(auto) eval_placeholder (I, T && arg)
        {
            static_assert(
                I::value == 1,
                "The current placeholder's index is out of bounds.  Did you perhaps call "
                "evaluate() with too few arguments?"
            );
            return static_cast<T &&>(arg);
        }

        template <typename I, typename T, typename ...Ts>
        auto eval_placeholder (I, T && arg, Ts &&... args)
        {
            if constexpr (I::value == 1) {
                return arg;
            } else {
                return eval_placeholder(hana::llong<I::value - 1>{}, static_cast<Ts &&>(args)...);
            }
        }

        template <typename Expr, typename ...T>
        decltype(auto) default_eval_expr (Expr const & expr, T &&... args)
        {
            constexpr expr_kind kind = Expr::kind;

            using namespace hana::literals;

            if constexpr (
                !std::is_same_v<
                    decltype(transform_expression(expr, static_cast<T &&>(args)...)),
                    nonexistent_transform
                >
            ) {
                return transform_expression(expr, static_cast<T &&>(args)...);
            } else if constexpr (kind == expr_kind::expr_ref) {
                return default_eval_expr(expr.value(), static_cast<T &&>(args)...);
            } else if constexpr (kind == expr_kind::terminal) {
                return expr.value();
            } else if constexpr (kind == expr_kind::placeholder) {
                return eval_placeholder(expr.value(), static_cast<T &&>(args)...);
            }

#define BOOST_PROTO17_UNARY_OPERATOR_CASE(op_name)                      \
            else if constexpr (kind == expr_kind:: op_name) {           \
                return                                                  \
                    eval_ ## op_name(                                   \
                        default_eval_expr(expr.elements[0_c], static_cast<T &&>(args)...) \
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

#undef BOOST_PROTO17_UNARY_OPERATOR_CASE

#define BOOST_PROTO17_BINARY_OPERATOR_CASE(op_name)                     \
            else if constexpr (kind == expr_kind:: op_name) {           \
                return                                                  \
                    eval_ ## op_name(                                   \
                        default_eval_expr(expr.elements[0_c], static_cast<T &&>(args)...), \
                        default_eval_expr(expr.elements[1_c], static_cast<T &&>(args)...) \
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

            else if constexpr (kind == expr_kind::comma) {
                return
                    eval_comma(
                        default_eval_expr(expr.elements[0_c], static_cast<T &&>(args)...),
                        default_eval_expr(expr.elements[1_c], static_cast<T &&>(args)...)
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
            BOOST_PROTO17_BINARY_OPERATOR_CASE(subscript) // []

#undef BOOST_PROTO17_BINARY_OPERATOR_CASE

            else if constexpr (kind == expr_kind::call) {
                auto expand_args = [&](auto && element) {
                    return default_eval_expr(
                        static_cast<decltype(element) &&>(element),
                        static_cast<T &&>(args)...
                    );
                };

                return hana::unpack(
                    expr.elements,
                    [expand_args](auto && ... elements) {
                        return eval_call(
                            expand_args(static_cast<decltype(elements) &&>(elements))...
                        );
                    });
            } else {
                assert(false && "Unhandled expr_kind in default_evaluate!");
                return;
            }
        }

        template <typename Expr, typename Tuple, typename Transform>
        auto transform_nonterminal (Expr const & expr, Tuple && tuple, Transform && transform);

        template <typename Expr, typename Transform, typename = std::void_t<>>
        struct default_transform_expression
        {
            auto operator() (Expr && expr, Transform && transform)
            {
                constexpr expr_kind kind = remove_cv_ref_t<Expr>::kind;
                if constexpr (kind == expr_kind::expr_ref) {
                    decltype(auto) ref = expr.value();
                    default_transform_expression<decltype(ref), Transform> transformer;
                    return transformer(ref, static_cast<Transform &&>(transform));
                } else if constexpr (kind == expr_kind::terminal || kind == expr_kind::placeholder) {
                    return static_cast<Expr &&>(expr);
                } else {
                    return transform_nonterminal(
                        expr,
                        static_cast<decltype(expr.elements) &&>(expr.elements),
                        static_cast<Transform &&>(transform)
                    );
                }
            }
        };

        template <typename Expr, typename Transform>
        struct default_transform_expression<
            Expr,
            Transform,
            std::void_t<decltype(std::declval<Transform>()(std::declval<Expr>()))>
        >
        {
            auto operator() (Expr && expr, Transform && transform)
            { return static_cast<Transform &&>(transform)(static_cast<Expr &&>(expr)); }
        };

        // TODO: Add a test that exercises this witht aome expression template
        // other than expression<>.
        template <
            template<expr_kind, class, class ...> class Expr,
            expr_kind Kind,
            typename OldTuple,
            typename NewTuple,
            typename ...T
        >
        auto make_expr_from_tuple (Expr<Kind, OldTuple, T...> const & expr, NewTuple && tuple)
        { return Expr<Kind, NewTuple, T...>(std::move(tuple)); }

        template <typename Expr, typename Tuple, typename Transform>
        auto transform_nonterminal (Expr const & expr, Tuple && tuple, Transform && transform)
        {
            auto transformed_tuple = hana::transform(
                static_cast<Tuple &&>(tuple),
                [&transform](auto && element) {
                    using element_t = decltype(element);
                    default_transform_expression<element_t, Transform> transformer;
                    return transformer(
                        static_cast<element_t &&>(element),
                        static_cast<Transform &&>(transform)
                    );
                }
            );
            return make_expr_from_tuple(expr, std::move(transformed_tuple));
        }

    }

}

#endif
