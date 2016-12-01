#ifndef BOOST_YAP_OPERATORS_HPP_INCLUDED
#define BOOST_YAP_OPERATORS_HPP_INCLUDED


namespace boost::yap {

#define BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(expr)                    \
    noexcept(noexcept(expr)) -> decltype(expr) { return expr; }

    namespace adl_detail {

#define BOOST_YAP_UNARY_OPERATOR(op, op_name)                       \
        template <typename T>                                           \
        constexpr auto eval_ ## op_name (T && t) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN( \
            op static_cast<T &&>(t)                                     \
        )                                                               \
        struct eval_ ## op_name ## _fn                                  \
        {                                                               \
            template <typename T>                                       \
            constexpr auto operator() (T && t) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN( \
                eval_ ## op_name(static_cast<T &&>(t))                  \
            )                                                           \
        };

        BOOST_YAP_UNARY_OPERATOR(+, unary_plus) // +
        BOOST_YAP_UNARY_OPERATOR(-, negate) // -
        BOOST_YAP_UNARY_OPERATOR(*, dereference) // *
        BOOST_YAP_UNARY_OPERATOR(~, complement) // ~
        BOOST_YAP_UNARY_OPERATOR(&, address_of) // &
        BOOST_YAP_UNARY_OPERATOR(!, logical_not) // !
        BOOST_YAP_UNARY_OPERATOR(++, pre_inc) // ++
        BOOST_YAP_UNARY_OPERATOR(--, pre_dec) // --

        template <typename T>
        constexpr auto eval_post_inc (T && t) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
            static_cast<T &&>(t) ++
        )
        struct eval_post_inc_fn
        {
            template <typename T>
            constexpr auto operator() (T && t) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
                eval_post_inc(static_cast<T &&>(t))
            )
        };

        template <typename T>
        constexpr auto eval_post_dec (T && t) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
            static_cast<T &&>(t) --
        )
        struct eval_post_dec_fn
        {
            template <typename T>
            constexpr auto operator() (T && t) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
                eval_post_dec(static_cast<T &&>(t))
            )
        };

#undef BOOST_YAP_UNARY_OPERATOR

#define BOOST_YAP_BINARY_OPERATOR(op, op_name)                      \
        template <typename T, typename U>                               \
        constexpr auto eval_ ## op_name (T && t, U && u) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN( \
            static_cast<T &&>(t) op static_cast<U &&>(u)                \
        )                                                               \
        struct eval_ ## op_name ## _fn                                  \
        {                                                               \
            template <typename T, typename U>                           \
            constexpr auto operator() (T && t, U && u) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN( \
                eval_ ## op_name(static_cast<T &&>(t), static_cast<U &&>(u)) \
            )                                                           \
        };

        BOOST_YAP_BINARY_OPERATOR(<<, shift_left) // <<
        BOOST_YAP_BINARY_OPERATOR(>>, shift_right) // >>
        BOOST_YAP_BINARY_OPERATOR(*, multiplies) // *
        BOOST_YAP_BINARY_OPERATOR(/, divides) // /
        BOOST_YAP_BINARY_OPERATOR(%, modulus) // %
        BOOST_YAP_BINARY_OPERATOR(+, plus) // +
        BOOST_YAP_BINARY_OPERATOR(-, minus) // -
        BOOST_YAP_BINARY_OPERATOR(<, less) // <
        BOOST_YAP_BINARY_OPERATOR(>, greater) // >
        BOOST_YAP_BINARY_OPERATOR(<=, less_equal) // <=
        BOOST_YAP_BINARY_OPERATOR(>=, greater_equal) // >=
        BOOST_YAP_BINARY_OPERATOR(==, equal_to) // ==
        BOOST_YAP_BINARY_OPERATOR(!=, not_equal_to) // !=
        BOOST_YAP_BINARY_OPERATOR(||, logical_or) // ||
        BOOST_YAP_BINARY_OPERATOR(&&, logical_and) // &&
        BOOST_YAP_BINARY_OPERATOR(&, bitwise_and) // &
        BOOST_YAP_BINARY_OPERATOR(|, bitwise_or) // |
        BOOST_YAP_BINARY_OPERATOR(^, bitwise_xor) // ^

        template <typename T, typename U>
        constexpr auto eval_comma (T && t, U && u) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
            (static_cast<T &&>(t) , static_cast<U &&>(u))
        )
        struct eval_comma_fn
        {
            template <typename T, typename U>
            constexpr auto operator() (T && t, U && u) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
                eval_comma(static_cast<T &&>(t), static_cast<U &&>(u))
            )
        };

        BOOST_YAP_BINARY_OPERATOR(->*, mem_ptr) // ->*
        BOOST_YAP_BINARY_OPERATOR(=, assign) // =
        BOOST_YAP_BINARY_OPERATOR(<<=, shift_left_assign) // <<=
        BOOST_YAP_BINARY_OPERATOR(>>=, shift_right_assign) // >>=
        BOOST_YAP_BINARY_OPERATOR(*=, multiplies_assign) // *=
        BOOST_YAP_BINARY_OPERATOR(/=, divides_assign) // /=
        BOOST_YAP_BINARY_OPERATOR(%=, modulus_assign) // %=
        BOOST_YAP_BINARY_OPERATOR(+=, plus_assign) // +=
        BOOST_YAP_BINARY_OPERATOR(-=, minus_assign) // -=
        BOOST_YAP_BINARY_OPERATOR(&=, bitwise_and_assign) // &=
        BOOST_YAP_BINARY_OPERATOR(|=, bitwise_or_assign) // |=
        BOOST_YAP_BINARY_OPERATOR(^=, bitwise_xor_assign) // ^=

        template <typename T, typename U>
        constexpr auto eval_subscript (T && t, U && u) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
            static_cast<T &&>(t)[static_cast<U &&>(u)]
        )
        struct eval_subscript_fn
        {
            template <typename T, typename U>
            constexpr auto operator() (T && t, U && u) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
                eval_subscript(static_cast<T &&>(t), static_cast<U &&>(u))
            )
        };

#undef BOOST_YAP_BINARY_OPERATOR

        template <typename T, typename U, typename V>
        constexpr auto eval_if_else (T && t, U && u, V && v) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
            static_cast<T &&>(t) ? static_cast<U &&>(u) : static_cast<V &&>(v)
        )
        struct eval_if_else_fn
        {
            template <typename T, typename U, typename V>
            constexpr auto operator() (T && t, U && u, V && v) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
                eval_if_else(static_cast<T &&>(t), static_cast<U &&>(u), static_cast<V &&>(v))
            )
        };

        template <typename F, typename ...T>
        constexpr auto eval_call (F && f, T && ...t) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
            static_cast<F &&>(f)(static_cast<T &&>(t)...)
        )
        struct eval_call_fn
        {
            template <typename F, typename ...T>
            constexpr auto operator() (F && f, T && ...t) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
                eval_call(static_cast<F &&>(f), static_cast<T &&>(t)...)
            )
        };

    }

#undef BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN

#define BOOST_YAP_USING_OPERATOR_FN(op_name) using adl_detail::eval_ ## op_name ## _fn;

    BOOST_YAP_USING_OPERATOR_FN(unary_plus) // +
    BOOST_YAP_USING_OPERATOR_FN(negate) // -
    BOOST_YAP_USING_OPERATOR_FN(dereference) // *
    BOOST_YAP_USING_OPERATOR_FN(complement) // ~
    BOOST_YAP_USING_OPERATOR_FN(address_of) // &
    BOOST_YAP_USING_OPERATOR_FN(logical_not) // !
    BOOST_YAP_USING_OPERATOR_FN(pre_inc) // ++
    BOOST_YAP_USING_OPERATOR_FN(pre_dec) // --
    BOOST_YAP_USING_OPERATOR_FN(post_inc) // ++(int)
    BOOST_YAP_USING_OPERATOR_FN(post_dec) // --(int)
    BOOST_YAP_USING_OPERATOR_FN(shift_left) // <<
    BOOST_YAP_USING_OPERATOR_FN(shift_right) // >>
    BOOST_YAP_USING_OPERATOR_FN(multiplies) // *
    BOOST_YAP_USING_OPERATOR_FN(divides) // /
    BOOST_YAP_USING_OPERATOR_FN(modulus) // %
    BOOST_YAP_USING_OPERATOR_FN(plus) // +
    BOOST_YAP_USING_OPERATOR_FN(minus) // -
    BOOST_YAP_USING_OPERATOR_FN(less) // <
    BOOST_YAP_USING_OPERATOR_FN(greater) // >
    BOOST_YAP_USING_OPERATOR_FN(less_equal) // <=
    BOOST_YAP_USING_OPERATOR_FN(greater_equal) // >=
    BOOST_YAP_USING_OPERATOR_FN(equal_to) // ==
    BOOST_YAP_USING_OPERATOR_FN(not_equal_to) // !=
    BOOST_YAP_USING_OPERATOR_FN(logical_or) // ||
    BOOST_YAP_USING_OPERATOR_FN(logical_and) // &&
    BOOST_YAP_USING_OPERATOR_FN(bitwise_and) // &
    BOOST_YAP_USING_OPERATOR_FN(bitwise_or) // |
    BOOST_YAP_USING_OPERATOR_FN(bitwise_xor) // ^
    BOOST_YAP_USING_OPERATOR_FN(comma) // ,
    BOOST_YAP_USING_OPERATOR_FN(mem_ptr) // ->*
    BOOST_YAP_USING_OPERATOR_FN(assign) // =
    BOOST_YAP_USING_OPERATOR_FN(shift_left_assign) // <<=
    BOOST_YAP_USING_OPERATOR_FN(shift_right_assign) // >>=
    BOOST_YAP_USING_OPERATOR_FN(multiplies_assign) // *=
    BOOST_YAP_USING_OPERATOR_FN(divides_assign) // /=
    BOOST_YAP_USING_OPERATOR_FN(modulus_assign) // %=
    BOOST_YAP_USING_OPERATOR_FN(plus_assign) // +=
    BOOST_YAP_USING_OPERATOR_FN(minus_assign) // -=
    BOOST_YAP_USING_OPERATOR_FN(bitwise_and_assign) // &=
    BOOST_YAP_USING_OPERATOR_FN(bitwise_or_assign) // |=
    BOOST_YAP_USING_OPERATOR_FN(bitwise_xor_assign) // ^=
    BOOST_YAP_USING_OPERATOR_FN(subscript) // []
    BOOST_YAP_USING_OPERATOR_FN(if_else) // (analogous to) ?:
    BOOST_YAP_USING_OPERATOR_FN(call) // ()

#undef BOOST_YAP_USING_OPERATOR_FN

    inline namespace function_objects {

#define BOOST_YAP_DECLARE_OPERATOR_FN(op_name)                        \
        inline constexpr eval_ ## op_name ## _fn eval_ ## op_name{};

        BOOST_YAP_DECLARE_OPERATOR_FN(unary_plus) // +
        BOOST_YAP_DECLARE_OPERATOR_FN(negate) // -
        BOOST_YAP_DECLARE_OPERATOR_FN(dereference) // *
        BOOST_YAP_DECLARE_OPERATOR_FN(complement) // ~
        BOOST_YAP_DECLARE_OPERATOR_FN(address_of) // &
        BOOST_YAP_DECLARE_OPERATOR_FN(logical_not) // !
        BOOST_YAP_DECLARE_OPERATOR_FN(pre_inc) // ++
        BOOST_YAP_DECLARE_OPERATOR_FN(pre_dec) // --
        BOOST_YAP_DECLARE_OPERATOR_FN(post_inc) // ++(int)
        BOOST_YAP_DECLARE_OPERATOR_FN(post_dec) // --(int)
        BOOST_YAP_DECLARE_OPERATOR_FN(shift_left) // <<
        BOOST_YAP_DECLARE_OPERATOR_FN(shift_right) // >>
        BOOST_YAP_DECLARE_OPERATOR_FN(multiplies) // *
        BOOST_YAP_DECLARE_OPERATOR_FN(divides) // /
        BOOST_YAP_DECLARE_OPERATOR_FN(modulus) // %
        BOOST_YAP_DECLARE_OPERATOR_FN(plus) // +
        BOOST_YAP_DECLARE_OPERATOR_FN(minus) // -
        BOOST_YAP_DECLARE_OPERATOR_FN(less) // <
        BOOST_YAP_DECLARE_OPERATOR_FN(greater) // >
        BOOST_YAP_DECLARE_OPERATOR_FN(less_equal) // <=
        BOOST_YAP_DECLARE_OPERATOR_FN(greater_equal) // >=
        BOOST_YAP_DECLARE_OPERATOR_FN(equal_to) // ==
        BOOST_YAP_DECLARE_OPERATOR_FN(not_equal_to) // !=
        BOOST_YAP_DECLARE_OPERATOR_FN(logical_or) // ||
        BOOST_YAP_DECLARE_OPERATOR_FN(logical_and) // &&
        BOOST_YAP_DECLARE_OPERATOR_FN(bitwise_and) // &
        BOOST_YAP_DECLARE_OPERATOR_FN(bitwise_or) // |
        BOOST_YAP_DECLARE_OPERATOR_FN(bitwise_xor) // ^
        BOOST_YAP_DECLARE_OPERATOR_FN(comma) // ,
        BOOST_YAP_DECLARE_OPERATOR_FN(mem_ptr) // ->*
        BOOST_YAP_DECLARE_OPERATOR_FN(assign) // =
        BOOST_YAP_DECLARE_OPERATOR_FN(shift_left_assign) // <<=
        BOOST_YAP_DECLARE_OPERATOR_FN(shift_right_assign) // >>=
        BOOST_YAP_DECLARE_OPERATOR_FN(multiplies_assign) // *=
        BOOST_YAP_DECLARE_OPERATOR_FN(divides_assign) // /=
        BOOST_YAP_DECLARE_OPERATOR_FN(modulus_assign) // %=
        BOOST_YAP_DECLARE_OPERATOR_FN(plus_assign) // +=
        BOOST_YAP_DECLARE_OPERATOR_FN(minus_assign) // -=
        BOOST_YAP_DECLARE_OPERATOR_FN(bitwise_and_assign) // &=
        BOOST_YAP_DECLARE_OPERATOR_FN(bitwise_or_assign) // |=
        BOOST_YAP_DECLARE_OPERATOR_FN(bitwise_xor_assign) // ^=
        BOOST_YAP_DECLARE_OPERATOR_FN(subscript) // []
        BOOST_YAP_DECLARE_OPERATOR_FN(if_else) // (analogous to) ?:
        BOOST_YAP_DECLARE_OPERATOR_FN(call) // ()

#undef BOOST_YAP_DECLARE_OPERATOR_FN

    }

}

#endif
