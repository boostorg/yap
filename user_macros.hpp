#ifndef BOOST_YAP_USER_MACROS_HPP_INCLUDED
#define BOOST_YAP_USER_MACROS_HPP_INCLUDED

#include <boost/preprocessor/cat.hpp>


// unary
#define BOOST_YAP_OPERATOR_unary_plus(param_list) + param_list
#define BOOST_YAP_OPERATOR_negate(param_list) - param_list
#define BOOST_YAP_OPERATOR_dereference(param_list) * param_list
#define BOOST_YAP_OPERATOR_complement(param_list) ~ param_list
#define BOOST_YAP_OPERATOR_address_of(param_list) & param_list
#define BOOST_YAP_OPERATOR_logical_not(param_list) ! param_list
#define BOOST_YAP_OPERATOR_pre_inc(param_list) ++ param_list
#define BOOST_YAP_OPERATOR_pre_dec(param_list) -- param_list
#define BOOST_YAP_OPERATOR_post_inc(param_list) ++ (int)
#define BOOST_YAP_OPERATOR_post_dec(param_list) -- (int)

// binary
#define BOOST_YAP_OPERATOR_shift_left() <<
#define BOOST_YAP_OPERATOR_shift_right() >>
#define BOOST_YAP_OPERATOR_multiplies() *
#define BOOST_YAP_OPERATOR_divides() /
#define BOOST_YAP_OPERATOR_modulus() %
#define BOOST_YAP_OPERATOR_plus() +
#define BOOST_YAP_OPERATOR_minus() -
#define BOOST_YAP_OPERATOR_less() <
#define BOOST_YAP_OPERATOR_greater() >
#define BOOST_YAP_OPERATOR_less_equal() <=
#define BOOST_YAP_OPERATOR_greater_equal() >=
#define BOOST_YAP_OPERATOR_equal_to() ==
#define BOOST_YAP_OPERATOR_not_equal_to() !=
#define BOOST_YAP_OPERATOR_logical_or() ||
#define BOOST_YAP_OPERATOR_logical_and() &&
#define BOOST_YAP_OPERATOR_bitwise_and() &
#define BOOST_YAP_OPERATOR_bitwise_or() |
#define BOOST_YAP_OPERATOR_bitwise_xor() ^
#define BOOST_YAP_OPERATOR_comma() ,
#define BOOST_YAP_OPERATOR_mem_ptr() ->*
#define BOOST_YAP_OPERATOR_assign() =
#define BOOST_YAP_OPERATOR_shift_left_assign() <<=
#define BOOST_YAP_OPERATOR_shift_right_assign() >>=
#define BOOST_YAP_OPERATOR_multiplies_assign() *=
#define BOOST_YAP_OPERATOR_divides_assign() /=
#define BOOST_YAP_OPERATOR_modulus_assign() %=
#define BOOST_YAP_OPERATOR_plus_assign() +=
#define BOOST_YAP_OPERATOR_minus_assign() -=
#define BOOST_YAP_OPERATOR_bitwise_and_assign() &=
#define BOOST_YAP_OPERATOR_bitwise_or_assign() |=
#define BOOST_YAP_OPERATOR_bitwise_xor_assign() ^=
#define BOOST_YAP_OPERATOR_subscript() []

#define BOOST_YAP_INDIRECT_CALL(macro) BOOST_PP_CAT(BOOST_YAP_OPERATOR_, macro)

#define BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(op_name, this_type, expr_template) \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)(()) const &      \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<lhs_type>;              \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{lhs_type{lhs_tuple_type{this}}}                  \
        };                                                              \
    }                                                                   \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)(()) &            \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<lhs_type>;              \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{lhs_type{lhs_tuple_type{this}}}                  \
        };                                                              \
    }                                                                   \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)(()) &&           \
    {                                                                   \
        using tuple_type = ::boost::hana::tuple<this_type>;             \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{std::move(*this)}                                \
        };                                                              \
    }

#define BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(op_name, this_type, expr_template) \
    template <typename Expr>                                            \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (Expr && rhs) const & \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using rhs_type = ::boost::yap::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::yap::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename Expr>                                            \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (Expr && rhs) & \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using rhs_type = ::boost::yap::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::yap::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename Expr>                                            \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (Expr && rhs) && \
    {                                                                   \
        using rhs_type = ::boost::yap::detail::operand_type_t<expr_template, Expr>; \
        using tuple_type = ::boost::hana::tuple<this_type, rhs_type>;   \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{                                                 \
                std::move(*this),                                       \
                ::boost::yap::detail::make_operand<rhs_type>{}(static_cast<Expr &&>(rhs)) \
            }                                                           \
        };                                                              \
    }

#define BOOST_YAP_USER_MEMBER_CALL_OPERATOR(this_type, expr_template) \
    template <typename ...U>                                            \
    auto operator() (U && ...u) const &                                 \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<                        \
            lhs_type,                                                   \
            ::boost::yap::detail::operand_type_t<expr_template, U>... \
        >;                                                              \
        return expr_template< ::boost::yap::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::yap::detail::make_operand<                 \
                    ::boost::yap::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename ...U>                                            \
    auto operator() (U && ...u) &                                       \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<                        \
            lhs_type,                                                   \
            ::boost::yap::detail::operand_type_t<expr_template, U>... \
        >;                                                              \
        return expr_template< ::boost::yap::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::yap::detail::make_operand<                 \
                    ::boost::yap::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename ...U>                                            \
    auto operator() (U && ...u) &&                                      \
    {                                                                   \
        using tuple_type = ::boost::hana::tuple<                        \
            this_type,                                                  \
            ::boost::yap::detail::operand_type_t<expr_template, U>... \
        >;                                                              \
        return expr_template< ::boost::yap::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                std::move(*this),                                       \
                ::boost::yap::detail::make_operand<                 \
                    ::boost::yap::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }


#define BOOST_YAP_USER_FREE_BINARY_OPERATOR(op_name, expr_template) \
    template <typename T, typename Expr>                                \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (T && lhs, Expr && rhs) \
        -> ::boost::yap::detail::free_binary_op_result_t<           \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                       \
            T,                                                          \
            Expr &&                                                     \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::free_binary_op_result< \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                       \
            T,                                                          \
            Expr &&                                                     \
        >;                                                              \
        using lhs_type = typename result_types::lhs_type;               \
        using rhs_type = typename result_types::rhs_type;               \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        if constexpr (std::is_lvalue_reference<Expr>{}) {               \
            using rhs_tuple_type = typename result_types::rhs_tuple_type; \
            return {                                                    \
                tuple_type{                                             \
                    lhs_type{static_cast<T &&>(lhs)},                   \
                    rhs_type{rhs_tuple_type{std::addressof(rhs)}}       \
                }                                                       \
            };                                                          \
        } else {                                                        \
            return {                                                    \
                tuple_type{                                             \
                    lhs_type{static_cast<T &&>(lhs)},                   \
                    std::move(rhs)                                      \
                }                                                       \
            };                                                          \
        }                                                               \
    }                                                                   \


#define BOOST_YAP_USER_EXPR_IF_ELSE(expr_template)                  \
    template <typename Expr1, typename Expr2, typename Expr3>           \
    auto if_else (Expr1 && expr1, Expr2 && expr2, Expr3 && expr3)       \
        -> ::boost::yap::detail::ternary_op_result_t<               \
            expr_template,                                              \
            Expr1,                                                      \
            Expr2,                                                      \
            Expr3                                                       \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::ternary_op_result< \
            expr_template,                                              \
            Expr1,                                                      \
            Expr2,                                                      \
            Expr3                                                       \
        >;                                                              \
        using cond_type = typename result_types::cond_type;             \
        using then_type = typename result_types::then_type;             \
        using else_type = typename result_types::else_type;             \
        using tuple_type = ::boost::hana::tuple<cond_type, then_type, else_type>; \
        return {                                                        \
            tuple_type{                                                 \
                ::boost::yap::detail::make_operand<cond_type>{}(static_cast<Expr1 &&>(expr1)), \
                ::boost::yap::detail::make_operand<then_type>{}(static_cast<Expr2 &&>(expr2)), \
                ::boost::yap::detail::make_operand<else_type>{}(static_cast<Expr3 &&>(expr3)) \
            }                                                           \
        };                                                              \
    }


#define BOOST_YAP_USER_UDT_ANY_IF_ELSE(expr_template, udt_trait)    \
    template <typename Expr1, typename Expr2, typename Expr3>           \
    auto if_else (Expr1 && expr1, Expr2 && expr2, Expr3 && expr3)       \
        -> ::boost::yap::detail::udt_any_ternary_op_result_t<       \
            expr_template,                                              \
            Expr1,                                                      \
            Expr2,                                                      \
            Expr3,                                                      \
            udt_trait                                                   \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::udt_any_ternary_op_result< \
            expr_template,                                              \
            Expr1,                                                      \
            Expr2,                                                      \
            Expr3,                                                      \
            udt_trait                                                   \
        >;                                                              \
        using cond_type = typename result_types::cond_type;             \
        using then_type = typename result_types::then_type;             \
        using else_type = typename result_types::else_type;             \
        using tuple_type = ::boost::hana::tuple<cond_type, then_type, else_type>; \
        return {                                                        \
            tuple_type{                                                 \
                ::boost::yap::detail::make_operand<cond_type>{}(static_cast<Expr1 &&>(expr1)), \
                ::boost::yap::detail::make_operand<then_type>{}(static_cast<Expr2 &&>(expr2)), \
                ::boost::yap::detail::make_operand<else_type>{}(static_cast<Expr3 &&>(expr3)) \
            }                                                           \
        };                                                              \
    }


#define BOOST_YAP_USER_UDT_UNARY_OPERATOR(op_name, expr_template, udt_trait) \
    template <typename T>                                               \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)((T && x))        \
        -> ::boost::yap::detail::udt_unary_op_result_t<             \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                       \
            T,                                                          \
            udt_trait                                                   \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::udt_unary_op_result< \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                       \
            T,                                                          \
            udt_trait                                                   \
        >;                                                              \
        using x_type = typename result_types::x_type;                   \
        using tuple_type = ::boost::hana::tuple<x_type>;                \
        return {tuple_type{x_type{static_cast<T &&>(x)}}};              \
    }                                                                   \


#define BOOST_YAP_USER_UDT_UDT_BINARY_OPERATOR(op_name, expr_template, t_udt_trait, u_udt_trait) \
    template <typename T, typename U>                                   \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (T && lhs, U && rhs) \
        -> ::boost::yap::detail::udt_udt_binary_op_result_t<        \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                       \
            T,                                                          \
            U,                                                          \
            t_udt_trait,                                                \
            u_udt_trait                                                 \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::udt_udt_binary_op_result< \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                       \
            T,                                                          \
            U,                                                          \
            t_udt_trait,                                                \
            u_udt_trait                                                 \
        >;                                                              \
        using lhs_type = typename result_types::lhs_type;               \
        using rhs_type = typename result_types::rhs_type;               \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return {                                                        \
            tuple_type{                                                 \
                lhs_type{static_cast<T &&>(lhs)},                       \
                rhs_type{static_cast<U &&>(rhs)},                       \
            }                                                           \
        };                                                              \
    }                                                                   \


#define BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(op_name, expr_template, udt_trait) \
    template <typename T, typename U>                                   \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (T && lhs, U && rhs) \
        -> ::boost::yap::detail::udt_any_binary_op_result_t<        \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                       \
            T,                                                          \
            U,                                                          \
            udt_trait                                                   \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::udt_any_binary_op_result< \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                       \
            T,                                                          \
            U,                                                          \
            udt_trait                                                   \
        >;                                                              \
        using lhs_type = typename result_types::lhs_type;               \
        using rhs_type = typename result_types::rhs_type;               \
        using tuple_type = ::boost::hana::tuple<lhs_type, rhs_type>;    \
        return {                                                        \
            tuple_type{                                                 \
                lhs_type{static_cast<T &&>(lhs)},                       \
                rhs_type{static_cast<U &&>(rhs)}                        \
            }                                                           \
        };                                                              \
    }                                                                   \

#endif
