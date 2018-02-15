#ifndef BOOST_YAP_ALGORITHM_FWD_HPP_INCLUDED
#define BOOST_YAP_ALGORITHM_FWD_HPP_INCLUDED

#include <boost/yap/config.hpp>

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/core/is_a.hpp>


namespace boost { namespace yap {

    /** The enumeration representing all the kinds of expressions supported in
        YAP.
    */
    enum class expr_kind {
        expr_ref = 0, ///< A (possibly \c const) reference to another expression.

        terminal = 1, ///< A terminal expression.

        // unary
        unary_plus = 2, ///< \c +
        negate = 3, ///< \c -
        dereference = 4, ///< \c *
        complement = 5, ///< \c ~
        address_of = 6, ///< \c &
        logical_not = 7, ///< \c !
        pre_inc = 8, ///< \c ++
        pre_dec = 9, ///< \c --
        post_inc = 10, ///< \c ++(int)
        post_dec = 11, ///< \c --(int)

        // binary
        shift_left = 12, ///< \c <<
        shift_right = 13, ///< \c >>
        multiplies = 14, ///< \c *
        divides = 15, ///< \c /
        modulus = 16, ///< \c %
        plus = 17, ///< \c +
        minus = 18, ///< \c -
        less = 19, ///< \c <
        greater = 20, ///< \c >
        less_equal = 21, ///< \c <=
        greater_equal = 22, ///< \c >=
        equal_to = 23, ///< \c ==
        not_equal_to = 24, ///< \c !=
        logical_or = 25, ///< \c ||
        logical_and = 26, ///< \c &&
        bitwise_and = 27, ///< \c &
        bitwise_or = 28, ///< \c |
        bitwise_xor = 29, ///< \c ^
        comma = 30, ///< \c  = ,
        mem_ptr = 31, ///< \c ->*
        assign = 32, ///< \c =
        shift_left_assign = 33, ///< \c <<=
        shift_right_assign = 34, ///< \c >>=
        multiplies_assign = 35, ///< \c *=
        divides_assign = 36, ///< \c /=
        modulus_assign = 37, ///< \c %=
        plus_assign = 38, ///< \c +=
        minus_assign = 39, ///< \c -=
        bitwise_and_assign = 40, ///< \c &=
        bitwise_or_assign = 41, ///< \c |=
        bitwise_xor_assign = 42, ///< \c ^=
        subscript = 43, ///< \c []

        // ternary
        if_else = 44, ///< Analogous to \c ?: .

        // n-ary
        call = 45 ///< \c ()
    };

    /** The type used to represent the index of a placeholder terminal. */
    template <long long I>
    struct placeholder : hana::llong<I> {};

#ifdef BOOST_YAP_DOXYGEN

    /** A metafunction that evaluates to std::true_type if \a Expr is an
        Expression, and std::false_type otherwise. */
    template <typename Expr>
    struct is_expr;

#else

    template <expr_kind Kind, typename Tuple>
    struct expression;

    namespace detail {

        // void_t

        template <class...> using void_t = void;

        // remove_cv_ref

        template <typename T>
        struct remove_cv_ref : std::remove_cv<std::remove_reference_t<T>>
        {};

        template <typename T>
        using remove_cv_ref_t = typename remove_cv_ref<T>::type;

    }

    template <typename Expr, typename = detail::void_t<>, typename = detail::void_t<>>
    struct is_expr : std::false_type {};

    template <typename Expr>
    struct is_expr<
        Expr,
        detail::void_t<decltype(detail::remove_cv_ref_t<Expr>::kind)>,
        detail::void_t<decltype(std::declval<Expr>().elements)>
    > : std::integral_constant<
        bool,
        std::is_same<std::remove_cv_t<decltype(detail::remove_cv_ref_t<Expr>::kind)>, expr_kind>{} &&
        hana::is_a<hana::tuple_tag, decltype(std::declval<Expr>().elements)>()
    > {};

#endif // BOOST_YAP_DOXYGEN

    /** A convenience alias for a terminal expression holding a \a T,
        instantiated from expression template \a expr_template. */
    template <template <expr_kind, class> class expr_template, typename T>
    using terminal = expr_template<expr_kind::terminal, hana::tuple<T>>;

    /** A convenience alias for a reference expression holding an expression
        \a T, instantiated from expression template \a expr_template. */
    template <template <expr_kind, class> class expr_template, typename T>
    using expression_ref = expr_template<expr_kind::expr_ref, hana::tuple<std::remove_reference_t<T> *>>;

#ifndef BOOST_YAP_DOXYGEN

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

#endif // BOOST_YAP_DOXYGEN

    namespace literals {

        /** Creates literal placeholders.  Placeholder indices are 1-based. */
        template <char ...c>
        constexpr auto operator"" _p ()
        {
            using i = hana::llong<hana::ic_detail::parse<sizeof...(c)>({c...})>;
            static_assert(1 <= i::value, "Placeholders must be >= 1.");
            return expression<expr_kind::terminal, hana::tuple<placeholder<i::value>>>{};
        }

    }

#ifndef BOOST_YAP_DOXYGEN

    // Tag types -- one per expr_kind:

    struct expr_ref_tag {};

    struct terminal_tag {};

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
