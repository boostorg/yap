#ifndef BOOST_PROTO17_USER_MACROS_HPP_INCLUDED
#define BOOST_PROTO17_USER_MACROS_HPP_INCLUDED


#define BOOST_PROTO17_USER_UNARY_OPERATOR_MEMBER(op, op_name, this_type, expr_template) \
    auto operator op const &                                            \
    {                                                                   \
        using lhs_type = ::boost::proto17::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::proto17::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<lhs_type>;              \
        return expr_template< ::boost::proto17::expr_kind::op_name, tuple_type>{ \
            tuple_type{lhs_type{lhs_tuple_type{this}}}                  \
        };                                                              \
    }                                                                   \
    auto operator op &                                                  \
    {                                                                   \
        using lhs_type = ::boost::proto17::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::proto17::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<lhs_type>;              \
        return expr_template< ::boost::proto17::expr_kind::op_name, tuple_type>{ \
            tuple_type{lhs_type{lhs_tuple_type{this}}}                  \
        };                                                              \
    }                                                                   \
    auto operator op &&                                                 \
    {                                                                   \
        using tuple_type = ::boost::hana::tuple<this_type>;             \
        return expr_template< ::boost::proto17::expr_kind::op_name, tuple_type>{ \
            tuple_type{std::move(*this)}                                \
        };                                                              \
    }

#define BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(op, op_name, this_type, expr_template) \
    template <typename Expr>                                            \
    auto operator op (Expr && rhs) const &                              \
    {                                                                   \
        using lhs_type = ::boost::proto17::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::proto17::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using rhs_type = ::boost::proto17::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return expr_template< ::boost::proto17::expr_kind::op_name, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::proto17::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename Expr>                                            \
    auto operator op (Expr && rhs) &                                    \
    {                                                                   \
        using lhs_type = ::boost::proto17::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::proto17::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using rhs_type = ::boost::proto17::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return expr_template< ::boost::proto17::expr_kind::op_name, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::proto17::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename Expr>                                            \
    auto operator op (Expr && rhs) &&                                   \
    {                                                                   \
        using rhs_type = ::boost::proto17::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<this_type, rhs_type>;   \
        return expr_template< ::boost::proto17::expr_kind::op_name, tuple_type>{ \
            tuple_type{                                                 \
                std::move(*this),                                       \
                ::boost::proto17::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }

#define BOOST_PROTO17_USER_MEMBER_COMMA_OPERATOR(this_type, expr_template) \
    template <typename Expr>                                            \
    auto operator, (Expr && rhs) const &                                \
    {                                                                   \
        using lhs_type = ::boost::proto17::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::proto17::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using rhs_type = ::boost::proto17::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return expr_template< ::boost::proto17::expr_kind::comma, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::proto17::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename Expr>                                            \
    auto operator, (Expr && rhs) &                                      \
    {                                                                   \
        using lhs_type = ::boost::proto17::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::proto17::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using rhs_type = ::boost::proto17::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return expr_template< ::boost::proto17::expr_kind::comma, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::proto17::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename Expr>                                            \
    auto operator, (Expr && rhs) &&                                     \
    {                                                                   \
        using rhs_type = ::boost::proto17::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<this_type, rhs_type>;   \
        return expr_template< ::boost::proto17::expr_kind::comma, tuple_type>{ \
            tuple_type{                                                 \
                std::move(*this),                                       \
                ::boost::proto17::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }

#define BOOST_PROTO17_USER_MEMBER_CALL_OPERATOR(this_type, expr_template) \
    template <typename ...U>                                            \
    auto operator() (U && ...u) const &                                 \
    {                                                                   \
        using lhs_type = ::boost::proto17::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::proto17::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<                        \
            lhs_type,                                                   \
            ::boost::proto17::detail::operand_type_t<expr_template, U>... \
        >;                                                              \
        return expr_template< ::boost::proto17::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::proto17::detail::make_operand<                 \
                    ::boost::proto17::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename ...U>                                            \
    auto operator() (U && ...u) &                                       \
    {                                                                   \
        using lhs_type = ::boost::proto17::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::proto17::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<                        \
            lhs_type,                                                   \
            ::boost::proto17::detail::operand_type_t<expr_template, U>... \
        >;                                                              \
        return expr_template< ::boost::proto17::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::proto17::detail::make_operand<                 \
                    ::boost::proto17::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename ...U>                                            \
    auto operator() (U && ...u) &&                                      \
    {                                                                   \
        using tuple_type = ::boost::hana::tuple<                        \
            this_type,                                                  \
            ::boost::proto17::detail::operand_type_t<expr_template, U>... \
        >;                                                              \
        return expr_template< ::boost::proto17::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                std::move(*this),                                       \
                ::boost::proto17::detail::make_operand<                 \
                    ::boost::proto17::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }


#define BOOST_PROTO17_USER_NON_MEMBER_BINARY_OPERATOR(op, op_name, expr_template) \
    template <typename T, typename Expr>                                \
    auto operator op (T && lhs, Expr const & rhs)                       \
        -> ::boost::proto17::detail::free_binary_op_result_t<           \
            expr_template,                                              \
            ::boost::proto17::expr_kind::op_name,                       \
            T,                                                          \
            Expr const &                                                \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::proto17::detail::free_binary_op_result< \
            expr_template,                                              \
            ::boost::proto17::expr_kind::op_name,                       \
            T,                                                          \
            Expr const &                                                \
        >;                                                              \
        using lhs_type = typename result_types::lhs_type;               \
        using rhs_type = typename result_types::rhs_type;               \
        using rhs_tuple_type = typename result_types::rhs_tuple_type;   \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return {                                                        \
            tuple_type{static_cast<T &&>(lhs), rhs_type{rhs_tuple_type{std::addressof(rhs)}}} \
        };                                                              \
    }                                                                   \
    template <typename T, typename Expr>                                \
    auto operator op (T && lhs, Expr & rhs)                             \
        -> ::boost::proto17::detail::free_binary_op_result_t<           \
            expr_template,                                              \
            ::boost::proto17::expr_kind::op_name,                       \
            T,                                                          \
            Expr &                                                      \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::proto17::detail::free_binary_op_result< \
            expr_template,                                              \
            ::boost::proto17::expr_kind::op_name,                       \
            T,                                                          \
            Expr &                                                      \
        >;                                                              \
        using lhs_type = typename result_types::lhs_type;               \
        using rhs_type = typename result_types::rhs_type;               \
        using rhs_tuple_type = typename result_types::rhs_tuple_type;   \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return {                                                        \
            tuple_type{static_cast<T &&>(lhs), rhs_type{rhs_tuple_type{std::addressof(rhs)}}} \
        };                                                              \
    }                                                                   \
    template <typename T, typename Expr>                                \
    auto operator op (T && lhs, ::boost::proto17::detail::remove_cv_ref_t<Expr> && rhs) \
        -> ::boost::proto17::detail::free_binary_op_result_t<           \
            expr_template,                                              \
            ::boost::proto17::expr_kind::op_name,                       \
            T,                                                          \
            ::boost::proto17::detail::remove_cv_ref_t<Expr>             \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::proto17::detail::free_binary_op_result< \
            expr_template,                                              \
            ::boost::proto17::expr_kind::op_name,                       \
            T,                                                          \
            ::boost::proto17::detail::remove_cv_ref_t<Expr>             \
        >;                                                              \
        using lhs_type = typename result_types::lhs_type;               \
        using rhs_type = typename result_types::rhs_type;               \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return {                                                        \
            tuple_type{static_cast<T &&>(lhs), std::move(rhs)}          \
        };                                                              \
    }

#endif
