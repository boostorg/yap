#ifndef BOOST_YAP_EXPRESSION_FWD_HPP_INCLUDED
#define BOOST_YAP_EXPRESSION_FWD_HPP_INCLUDED

#include <boost/yap/config.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>


namespace boost { namespace yap {

    /** The enumeration representing all the kinds of expressions supported in
        YAP.
    */
    enum class expr_kind {
        expr_ref, ///< An (possibly \c const) reference to another expression.

        terminal, ///< A terminal expression.
        placeholder, ///< A placeholder expression.

        // unary
        unary_plus, ///< \c +
        negate, ///< \c -
        dereference, ///< \c *
        complement, ///< \c ~
        address_of, ///< \c &
        logical_not, ///< \c !
        pre_inc, ///< \c ++
        pre_dec, ///< \c --
        post_inc, ///< \c ++(int)
        post_dec, ///< \c --(int)

        // binary
        shift_left, ///< \c <<
        shift_right, ///< \c >>
        multiplies, ///< \c *
        divides, ///< \c /
        modulus, ///< \c %
        plus, ///< \c +
        minus, ///< \c -
        less, ///< \c <
        greater, ///< \c >
        less_equal, ///< \c <=
        greater_equal, ///< \c >=
        equal_to, ///< \c ==
        not_equal_to, ///< \c !=
        logical_or, ///< \c ||
        logical_and, ///< \c &&
        bitwise_and, ///< \c &
        bitwise_or, ///< \c |
        bitwise_xor, ///< \c ^
        comma, ///< \c ,
        mem_ptr, ///< \c ->*
        assign, ///< \c =
        shift_left_assign, ///< \c <<=
        shift_right_assign, ///< \c >>=
        multiplies_assign, ///< \c *=
        divides_assign, ///< \c /=
        modulus_assign, ///< \c %=
        plus_assign, ///< \c +=
        minus_assign, ///< \c -=
        bitwise_and_assign, ///< \c &=
        bitwise_or_assign, ///< \c |=
        bitwise_xor_assign, ///< \c ^=
        subscript, ///< \c []

        // ternary
        if_else, ///< Analogous to \c ?: .

        // n-ary
        call ///< \c ()
    };

#ifndef BOOST_YAP_DOXYGEN

    template <expr_kind Kind, typename Tuple>
    struct expression;

#ifndef BOOST_YAP_DOXYGEN

    /** A convenience alias for a terminal expression holding a \a T,
        instantiated from expression template \a expr_template. */
    template <template <expr_kind, class> class expr_template, typename T>
    using terminal = expr_template<expr_kind::terminal, hana::tuple<T>>;

    /** A convenience alias for a reference expression holding an expression
        \a T, instantiated from expression template \a expr_template. */
    template <template <expr_kind, class> class expr_template, typename T>
    using expression_ref = expr_template<expr_kind::expr_ref, hana::tuple<std::remove_reference_t<T> *>>;

#endif // BOOST_YAP_DOXYGEN

    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr && expr, T && ... t);

    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr && expr, T && ... t);

    template <typename Expr, typename Transform>
    decltype(auto) transform (Expr && expr, Transform && transform);

    template <typename T>
    decltype(auto) deref (T && x);

    template <typename Expr>
    decltype(auto) value (Expr && expr);

    template <typename Expr>
    decltype(auto) left (Expr && expr);

    template <typename Expr>
    decltype(auto) right (Expr && expr);

#endif // BOOST_YAP_DOXYGEN

    namespace literals {

        /** Creates literal placeholders.  Placeholder indices are 1-based. */
        template <char ...c>
        constexpr auto operator"" _p ()
        {
            using i = hana::llong<hana::ic_detail::parse<sizeof...(c)>({c...})>;
            static_assert(1 <= i::value, "Placeholders must be >= 1.");
            return expression<expr_kind::placeholder, hana::tuple<i>>(i{});
        }

    }

#ifndef BOOST_YAP_DOXYGEN

    // Tag types -- one per expr_kind:

    struct expr_ref_tag {};

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

    // ternary
    struct if_else_tag {}; // Analogous to ?:

    // n-ary
    struct call_tag {}; // ()

#endif // BOOST_YAP_DOXYGEN

} }

#endif
