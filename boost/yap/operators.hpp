#ifndef BOOST_YAP_OPERATORS_HPP_INCLUDED
#define BOOST_YAP_OPERATORS_HPP_INCLUDED


namespace boost { namespace yap {

#ifndef BOOST_YAP_DOXYGEN

#define BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(expr)                        \
    noexcept(noexcept(expr)) -> decltype(expr) { return expr; }

    namespace adl_detail {

#define BOOST_YAP_UNARY_OPERATOR(op, op_name)                           \
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

#define BOOST_YAP_BINARY_OPERATOR(op, op_name)                          \
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
        constexpr auto eval_call (F && f, T && ... t) BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
            static_cast<F &&>(f)(static_cast<T &&>(t)...)
        )
        struct eval_call_fn
        {
            template <typename F, typename ...T>
            constexpr auto operator() (F && f, T && ... t) const BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN(
                eval_call(static_cast<F &&>(f), static_cast<T &&>(t)...)
            )
        };

    }

#undef BOOST_YAP_NOEXCEPT_DECLTYPE_RETURN

    using adl_detail::eval_unary_plus_fn; // +
    using adl_detail::eval_negate_fn; // -
    using adl_detail::eval_dereference_fn; // *
    using adl_detail::eval_complement_fn; // ~
    using adl_detail::eval_address_of_fn; // &
    using adl_detail::eval_logical_not_fn; // !
    using adl_detail::eval_pre_inc_fn; // ++
    using adl_detail::eval_pre_dec_fn; // --
    using adl_detail::eval_post_inc_fn; // ++(int)
    using adl_detail::eval_post_dec_fn; // --(int)
    using adl_detail::eval_shift_left_fn; // <<
    using adl_detail::eval_shift_right_fn; // >>
    using adl_detail::eval_multiplies_fn; // *
    using adl_detail::eval_divides_fn; // /
    using adl_detail::eval_modulus_fn; // %
    using adl_detail::eval_plus_fn; // +
    using adl_detail::eval_minus_fn; // -
    using adl_detail::eval_less_fn; // <
    using adl_detail::eval_greater_fn; // >
    using adl_detail::eval_less_equal_fn; // <=
    using adl_detail::eval_greater_equal_fn; // >=
    using adl_detail::eval_equal_to_fn; // ==
    using adl_detail::eval_not_equal_to_fn; // !=
    using adl_detail::eval_logical_or_fn; // ||
    using adl_detail::eval_logical_and_fn; // &&
    using adl_detail::eval_bitwise_and_fn; // &
    using adl_detail::eval_bitwise_or_fn; // |
    using adl_detail::eval_bitwise_xor_fn; // ^
    using adl_detail::eval_comma_fn; // ,
    using adl_detail::eval_mem_ptr_fn; // ->*
    using adl_detail::eval_assign_fn; // =
    using adl_detail::eval_shift_left_assign_fn; // <<=
    using adl_detail::eval_shift_right_assign_fn; // >>=
    using adl_detail::eval_multiplies_assign_fn; // *=
    using adl_detail::eval_divides_assign_fn; // /=
    using adl_detail::eval_modulus_assign_fn; // %=
    using adl_detail::eval_plus_assign_fn; // +=
    using adl_detail::eval_minus_assign_fn; // -=
    using adl_detail::eval_bitwise_and_assign_fn; // &=
    using adl_detail::eval_bitwise_or_assign_fn; // |=
    using adl_detail::eval_bitwise_xor_assign_fn; // ^=
    using adl_detail::eval_subscript_fn; // []
    using adl_detail::eval_if_else_fn; // (analogous to) ?:
    using adl_detail::eval_call_fn; // ()

    inline namespace {

        constexpr auto & eval_unary_plus = detail::static_const<eval_unary_plus_fn>::value; // +
        constexpr auto & eval_negate = detail::static_const<eval_negate_fn>::value; // -
        constexpr auto & eval_dereference = detail::static_const<eval_dereference_fn>::value; // *
        constexpr auto & eval_complement = detail::static_const<eval_complement_fn>::value; // ~
        constexpr auto & eval_address_of = detail::static_const<eval_address_of_fn>::value; // &
        constexpr auto & eval_logical_not = detail::static_const<eval_logical_not_fn>::value; // !
        constexpr auto & eval_pre_inc = detail::static_const<eval_pre_inc_fn>::value; // ++
        constexpr auto & eval_pre_dec = detail::static_const<eval_pre_dec_fn>::value; // --
        constexpr auto & eval_post_inc = detail::static_const<eval_post_inc_fn>::value; // ++(int)
        constexpr auto & eval_post_dec = detail::static_const<eval_post_dec_fn>::value; // --(int)
        constexpr auto & eval_shift_left = detail::static_const<eval_shift_left_fn>::value; // <<
        constexpr auto & eval_shift_right = detail::static_const<eval_shift_right_fn>::value; // >>
        constexpr auto & eval_multiplies = detail::static_const<eval_multiplies_fn>::value; // *
        constexpr auto & eval_divides = detail::static_const<eval_divides_fn>::value; // /
        constexpr auto & eval_modulus = detail::static_const<eval_modulus_fn>::value; // %
        constexpr auto & eval_plus = detail::static_const<eval_plus_fn>::value; // +
        constexpr auto & eval_minus = detail::static_const<eval_minus_fn>::value; // -
        constexpr auto & eval_less = detail::static_const<eval_less_fn>::value; // <
        constexpr auto & eval_greater = detail::static_const<eval_greater_fn>::value; // >
        constexpr auto & eval_less_equal = detail::static_const<eval_less_equal_fn>::value; // <=
        constexpr auto & eval_greater_equal = detail::static_const<eval_greater_equal_fn>::value; // >=
        constexpr auto & eval_equal_to = detail::static_const<eval_equal_to_fn>::value; // ==
        constexpr auto & eval_not_equal_to = detail::static_const<eval_not_equal_to_fn>::value; // !=
        constexpr auto & eval_logical_or = detail::static_const<eval_logical_or_fn>::value; // ||
        constexpr auto & eval_logical_and = detail::static_const<eval_logical_and_fn>::value; // &&
        constexpr auto & eval_bitwise_and = detail::static_const<eval_bitwise_and_fn>::value; // &
        constexpr auto & eval_bitwise_or = detail::static_const<eval_bitwise_or_fn>::value; // |
        constexpr auto & eval_bitwise_xor = detail::static_const<eval_bitwise_xor_fn>::value; // ^
        constexpr auto & eval_comma = detail::static_const<eval_comma_fn>::value; // ,
        constexpr auto & eval_mem_ptr = detail::static_const<eval_mem_ptr_fn>::value; // ->*
        constexpr auto & eval_assign = detail::static_const<eval_assign_fn>::value; // =
        constexpr auto & eval_shift_left_assign = detail::static_const<eval_shift_left_assign_fn>::value; // <<=
        constexpr auto & eval_shift_right_assign = detail::static_const<eval_shift_right_assign_fn>::value; // >>=
        constexpr auto & eval_multiplies_assign = detail::static_const<eval_multiplies_assign_fn>::value; // *=
        constexpr auto & eval_divides_assign = detail::static_const<eval_divides_assign_fn>::value; // /=
        constexpr auto & eval_modulus_assign = detail::static_const<eval_modulus_assign_fn>::value; // %=
        constexpr auto & eval_plus_assign = detail::static_const<eval_plus_assign_fn>::value; // +=
        constexpr auto & eval_minus_assign = detail::static_const<eval_minus_assign_fn>::value; // -=
        constexpr auto & eval_bitwise_and_assign = detail::static_const<eval_bitwise_and_assign_fn>::value; // &=
        constexpr auto & eval_bitwise_or_assign = detail::static_const<eval_bitwise_or_assign_fn>::value; // |=
        constexpr auto & eval_bitwise_xor_assign = detail::static_const<eval_bitwise_xor_assign_fn>::value; // ^=
        constexpr auto & eval_subscript = detail::static_const<eval_subscript_fn>::value; // []
        constexpr auto & eval_if_else = detail::static_const<eval_if_else_fn>::value; // (analogous to) ?:
        constexpr auto & eval_call = detail::static_const<eval_call_fn>::value; // ()

    }

#else // BOOST_YAP_DOXYGEN

    namespace function_objects {

        template <typename T>
        constexpr auto transform_expression (T && t);

        template <typename T>
        constexpr auto eval_unary_plus (T && t);
        template <typename T>
        constexpr auto eval_negate (T && t);
        template <typename T>
        constexpr auto eval_dereference (T && t);
        template <typename T>
        constexpr auto eval_complement (T && t);
        template <typename T>
        constexpr auto eval_address_of (T && t);
        template <typename T>
        constexpr auto eval_logical_not (T && t);
        template <typename T>
        constexpr auto eval_pre_inc (T && t);
        template <typename T>
        constexpr auto eval_pre_dec (T && t);
        template <typename T>
        constexpr auto eval_post_inc (T && t);
        template <typename T>
        constexpr auto eval_post_dec (T && t);
        template <typename T, typename U>
        constexpr auto eval_shift_left (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_shift_right (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_multiplies (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_divides (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_modulus (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_plus (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_minus (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_less (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_greater (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_less_equal (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_greater_equal (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_equal_to (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_not_equal_to (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_logical_or (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_logical_and (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_bitwise_and (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_bitwise_or (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_bitwise_xor (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_comma (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_mem_ptr (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_shift_left_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_shift_right_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_multiplies_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_divides_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_modulus_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_plus_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_minus_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_bitwise_and_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_bitwise_or_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_bitwise_xor_assign (T && t, U && u);
        template <typename T, typename U>
        constexpr auto eval_subscript (T && t, U && u);
        template <typename T, typename U, typename V>
        constexpr auto eval_if_else (T && t, U && u, V && v);
        template <typename F, typename ...T>
        constexpr auto eval_call (F && f, T && ... t);

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T && ... args);

    }

#endif // BOOST_YAP_DOXYGEN

} }

#endif
