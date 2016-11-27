#ifndef BOOST_PROTO17_EXPRESSION_FWD_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_FWD_HPP_INCLUDED

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>


namespace boost::proto17 {

    enum class expr_kind {
        expr_ref,

        terminal,
        placeholder,

        // unary
        unary_plus, // +
        negate, // -
        dereference, // *
        complement, // ~
        address_of, // &
        logical_not, // !
        pre_inc, // ++
        pre_dec, // --
        post_inc, // ++(int)
        post_dec, // --(int)

        // binary
        shift_left, // <<
        shift_right, // >>
        multiplies, // *
        divides, // /
        modulus, // %
        plus, // +
        minus, // -
        less, // <
        greater, // >
        less_equal, // <=
        greater_equal, // >=
        equal_to, // ==
        not_equal_to, // !=
        logical_or, // ||
        logical_and, // &&
        bitwise_and, // &
        bitwise_or, // |
        bitwise_xor, // ^
        comma, // ,
        mem_ptr, // ->*
        assign, // =
        shift_left_assign, // <<=
        shift_right_assign, // >>=
        multiplies_assign, // *=
        divides_assign, // /=
        modulus_assign, // %=
        plus_assign, // +=
        minus_assign, // -=
        bitwise_and_assign, // &=
        bitwise_or_assign, // |=
        bitwise_xor_assign, // ^=
        subscript, // []

        // n-ary
        call // ()
    };

    template <expr_kind Kind, typename Tuple>
    struct expression;

    template <typename T, template <expr_kind, class> class expr_template = expression>
    using terminal = expr_template<expr_kind::terminal, hana::tuple<T>>;

    template <typename T, template <expr_kind, class> class expr_template = expression>
    using expression_ref = expr_template<expr_kind::expr_ref, hana::tuple<std::remove_reference_t<T> *>>;

    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr const & expr, T && ...t);

    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr const & expr, T && ...t);

    template <typename Expr, typename Transform>
    auto transform (Expr && expr, Transform && transform);

    template <typename Expr>
    decltype(auto) value (Expr const & expr);

    template <typename Expr>
    decltype(auto) value (Expr & expr);

    template <typename Expr>
    decltype(auto) value (std::remove_reference_t<Expr> && expr);

    template <typename Expr>
    decltype(auto) left (Expr const & expr);

    template <typename Expr>
    decltype(auto) left (Expr & expr);

    template <typename Expr>
    decltype(auto) left (std::remove_reference_t<Expr> && expr);

    template <typename Expr>
    decltype(auto) right (Expr const & expr);

    template <typename Expr>
    decltype(auto) right (Expr & expr);;

    template <typename Expr>
    decltype(auto) right (std::remove_reference_t<Expr> && expr);

    namespace literals {

        template <char ...c>
        constexpr auto operator"" _p ()
        {
            using i = hana::llong<hana::ic_detail::parse<sizeof...(c)>({c...})>;
            static_assert(1 <= i::value, "Placeholders must be >= 1.");
            return expression<expr_kind::placeholder, hana::tuple<i>>(i{});
        }

    }

    // Tag types -- one per expr_kind:

    struct expr_ref_tag {}; // TODO: Needed?

    struct terminal_tag {};
    struct placeholder_tag {};

    // unary
    struct unary_plus_tag {}; // +
    struct negate_tag {}; // -
    struct dereference_tag {}; // *
    struct complement_tag {}; // ~
    struct address_of_tag {}; // &
    struct logical_not_tag {}; // !
    struct pre_inc_tag {}; // ++
    struct pre_dec_tag {}; // --
    struct post_inc_tag {}; // ++(int)
    struct post_dec_tag {}; // --(int)

    // binary
    struct shift_left_tag {}; // <<
    struct shift_right_tag {}; // >>
    struct multiplies_tag {}; // *
    struct divides_tag {}; // /
    struct modulus_tag {}; // %
    struct plus_tag {}; // +
    struct minus_tag {}; // -
    struct less_tag {}; // <
    struct greater_tag {}; // >
    struct less_equal_tag {}; // <=
    struct greater_equal_tag {}; // >=
    struct equal_to_tag {}; // ==
    struct not_equal_to_tag {}; // !=
    struct logical_or_tag {}; // ||
    struct logical_and_tag {}; // &&
    struct bitwise_and_tag {}; // &
    struct bitwise_or_tag {}; // |
    struct bitwise_xor_tag {}; // ^
    struct comma_tag {}; // _tag {};
    struct mem_ptr_tag {}; // ->*
    struct assign_tag {}; // =
    struct shift_left_assign_tag {}; // <<=
    struct shift_right_assign_tag {}; // >>=
    struct multiplies_assign_tag {}; // *=
    struct divides_assign_tag {}; // /=
    struct modulus_assign_tag {}; // %=
    struct plus_assign_tag {}; // +=
    struct minus_assign_tag {}; // -=
    struct bitwise_and_assign_tag {}; // &=
    struct bitwise_or_assign_tag {}; // |=
    struct bitwise_xor_assign_tag {}; // ^=
    struct subscript_tag {}; // []

    // n-ary
    struct call_tag {}; // ()

}

#endif
