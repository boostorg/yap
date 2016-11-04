#ifndef BOOST_PROTO17_OPERATORS_HPP_INCLUDED
#define BOOST_PROTO17_OPERATORS_HPP_INCLUDED


namespace boost::proto17 {

#define BOOST_PROTO17_NOEXCEPT_DECLTYPE_RETURN(expr)                    \
    noexcept(noexcept(expr)) -> decltype(expr) { return expr; }

    namespace adl_detail {

        template <typename T, typename U>
        constexpr auto eval_plus (T && t, U && u) BOOST_PROTO17_NOEXCEPT_DECLTYPE_RETURN(
            static_cast<T &&>(t) + static_cast<U &&>(u)
        )

        struct eval_plus_fn
        {
            template <typename T, typename U>
            constexpr auto operator() (T && t, U && u) const BOOST_PROTO17_NOEXCEPT_DECLTYPE_RETURN(
                eval_plus(static_cast<T &&>(t), static_cast<U &&>(u))
            )
        };

        template <typename T, typename U>
        constexpr auto eval_minus (T && t, U && u) BOOST_PROTO17_NOEXCEPT_DECLTYPE_RETURN(
            static_cast<T &&>(t) - static_cast<U &&>(u)
        )

        struct eval_minus_fn
        {
            template <typename T, typename U>
            constexpr auto operator() (T && t, U && u) const BOOST_PROTO17_NOEXCEPT_DECLTYPE_RETURN(
                eval_minus(static_cast<T &&>(t), static_cast<U &&>(u))
            )
        };

    }

#undef BOOST_PROTO17_NOEXCEPT_DECLTYPE_RETURN

    using adl_detail::eval_plus_fn;
    using adl_detail::eval_minus_fn;

    inline namespace function_objects {

        inline constexpr eval_plus_fn eval_plus{};
        inline constexpr eval_minus_fn eval_minus{};

    }

}

#endif
