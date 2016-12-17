#ifndef BOOST_YAP_USER_MACROS_HPP_INCLUDED
#define BOOST_YAP_USER_MACROS_HPP_INCLUDED

#include <boost/preprocessor/cat.hpp>


#ifndef BOOST_YAP_DOXYGEN

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

#endif // BOOST_YAP_DOXYGEN


/** Defines operator overloads for unary operator \a op_name that each produce
    an expression instantiated from the \a expr_template expression template.
    One overload is defined for each of the qualifiers <code>const &</code>,
    <code>&</code>, and <code>&&</code>.  For the lvalue reference overloads,
    <code>*this</code> is captured by reference into the resulting expression.
    For the rvalue reference overload, <code>*this</code> is moved into the
    resulting expression.

    Example:
    \snippet user_macros_snippets.cpp USER_UNARY_OPERATOR_MEMBER

    \param op_name The operator to be overloaded; this must be one of the \b
    unary enumerators in <code>expr_kind</code>, without the
    <code>expr_kind::</code> qualification.

    \param this_type The type in which the operator overloads are being
    defined.

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.
*/
#define BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(op_name, this_type, expr_template) \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)(()) const &          \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<lhs_type>;              \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{lhs_type{lhs_tuple_type{this}}}                  \
        };                                                              \
    }                                                                   \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)(()) &                \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<lhs_type>;              \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{lhs_type{lhs_tuple_type{this}}}                  \
        };                                                              \
    }                                                                   \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)(()) &&               \
    {                                                                   \
        using tuple_type = ::boost::hana::tuple<this_type>;             \
        return expr_template< ::boost::yap::expr_kind::op_name, tuple_type>{ \
            tuple_type{std::move(*this)}                                \
        };                                                              \
    }


/** Defines operator overloads for binary operator \a op_name that each
    produce an expression instantiated from the \a expr_template expression
    template.  One overload is defined for each of the qualifiers <code>const
    &</code>, <code>&</code>, and <code>&&</code>.  For the lvalue reference
    overloads, <code>*this</code> is captured by reference into the resulting
    expression.  For the rvalue reference overload, <code>*this</code> is
    moved into the resulting expression.

    The \a rhs parameter to each of the defined overloads may be any type,
    including an expression.  If \a rhs is a non-expression, it is wrapped in
    a terminal expression.

    Example:
    \snippet user_macros_snippets.cpp USER_BINARY_OPERATOR_MEMBER

    \param op_name The operator to be overloaded; this must be one of the \b
    binary enumerators in <code>expr_kind</code>, without the
    <code>expr_kind::</code> qualification.

    \param this_type The type in which the operator overloads are being
    defined.

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.
*/
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
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (Expr && rhs) &    \
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
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (Expr && rhs) &&   \
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


/** Defines operator overloads for the call operator ("operator()") that each
    produce an expression instantiated from the \a expr_template expression
    template.  One overload is defined for each of the qualifiers <code>const
    &</code>, <code>&</code>, and <code>&&</code>.  For the lvalue reference
    overloads, <code>*this</code> is captured by reference into the resulting
    expression.  For the rvalue reference overload, <code>*this</code> is
    moved into the resulting expression.

    The \a u parameters to each of the defined overloads may be any type,
    including an expression.  Each non-expression is wrapped in a terminal
    expression.

    Example:
    \snippet user_macros_snippets.cpp USER_MEMBER_CALL_OPERATOR

    \param this_type The type in which the operator overloads are being
    defined.

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.
*/
#define BOOST_YAP_USER_MEMBER_CALL_OPERATOR(this_type, expr_template)   \
    template <typename ...U>                                            \
    auto operator() (U && ... u) const &                                \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type const &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<                        \
            lhs_type,                                                   \
            ::boost::yap::detail::operand_type_t<expr_template, U>...   \
        >;                                                              \
        return expr_template< ::boost::yap::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::yap::detail::make_operand<                     \
                    ::boost::yap::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename ...U>                                            \
    auto operator() (U && ... u) &                                      \
    {                                                                   \
        using lhs_type = ::boost::yap::detail::expr_ref_t<expr_template, this_type &>; \
        using lhs_tuple_type = ::boost::yap::detail::expr_ref_tuple_t<expr_template, lhs_type>; \
        using tuple_type = ::boost::hana::tuple<                        \
            lhs_type,                                                   \
            ::boost::yap::detail::operand_type_t<expr_template, U>...   \
        >;                                                              \
        return expr_template< ::boost::yap::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                lhs_type{lhs_tuple_type{this}},                         \
                ::boost::yap::detail::make_operand<                     \
                    ::boost::yap::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }                                                                   \
    template <typename ...U>                                            \
    auto operator() (U && ... u) &&                                     \
    {                                                                   \
        using tuple_type = ::boost::hana::tuple<                        \
            this_type,                                                  \
            ::boost::yap::detail::operand_type_t<expr_template, U>...   \
        >;                                                              \
        return expr_template< ::boost::yap::expr_kind::call, tuple_type>{ \
            tuple_type{                                                 \
                std::move(*this),                                       \
                ::boost::yap::detail::make_operand<                     \
                    ::boost::yap::detail::operand_type_t<expr_template, U> \
                >{}(static_cast<U &&>(u))...                            \
            }                                                           \
        };                                                              \
    }


/** Defines a free/non-member operator overload for binary operator \a op_name
    that produces an expression instantiated from the \a expr_template
    expression template.

    The \a lhs parameter to each of the defined overloads must \b not be an
    expression; \a lhs is wrapped in a terminal expression.

    The \a rhs parameter to each of the defined overloads must be an
    expression.

    Example:
    \snippet user_macros_snippets.cpp USER_FREE_BINARY_OPERATOR

    \param op_name The operator to be overloaded; this must be one of the
    \binary enumerators in <code>expr_kind</code>, without the
    <code>expr_kind::</code> qualification.

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.
*/
#define BOOST_YAP_USER_FREE_BINARY_OPERATOR(op_name, expr_template)     \
    template <typename T, typename Expr>                                \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (T && lhs, Expr && rhs) \
        -> ::boost::yap::detail::free_binary_op_result_t<               \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                           \
            T,                                                          \
            Expr &&                                                     \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::free_binary_op_result< \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                           \
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
    }


/** Defines a function <code>if_else()</code> that acts as an analogue to the
    ternary operator (<code>?:</code>), since the ternary operator is not
    user-overloadable.  The return type of <code>if_else()</code> is an
    expression instantiated from the \a expr_template expression template.

    At least one parameter to <code>if_else()</code> must be an expression.

    For each parameter E passed to <code>if_else()</code>, if E is an rvalue,
    E is moved into the result, and otherwise E is captured by reference into
    the result.

    Example:
    \snippet user_macros_snippets.cpp USER_EXPR_IF_ELSE

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.
*/
#define BOOST_YAP_USER_EXPR_IF_ELSE(expr_template)                      \
    template <typename Expr1, typename Expr2, typename Expr3>           \
    auto if_else (Expr1 && expr1, Expr2 && expr2, Expr3 && expr3)       \
        -> ::boost::yap::detail::ternary_op_result_t<                   \
            expr_template,                                              \
            Expr1,                                                      \
            Expr2,                                                      \
            Expr3                                                       \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::ternary_op_result<   \
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


/** Defines a function <code>if_else()</code> that acts as an analogue to the
    ternary operator (<code>?:</code>), since the ternary operator is not
    user-overloadable.  The return type of <code>if_else()</code> is an
    expression instantiated from the \a expr_template expression template.

    Each parameter to <code>if_else()</code> may be any type that is \b not an
    expression.  At least on parameter must be a type <code>T</code> for which
    \code udt_trait<std::remove_cv_t<std::remove_reference_t<T>>>::value
    \endcode is true.  Each parameter is wrapped in a terminal expression.

    Example:
    \snippet user_macros_snippets.cpp USER_UDT_ANY_IF_ELSE

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.

    \param udt_trait A trait template to use to constrain which types are
    accepted as template parameters to <code>if_else()</code>.
*/
#define BOOST_YAP_USER_UDT_ANY_IF_ELSE(expr_template, udt_trait)        \
    template <typename Expr1, typename Expr2, typename Expr3>           \
    auto if_else (Expr1 && expr1, Expr2 && expr2, Expr3 && expr3)       \
        -> ::boost::yap::detail::udt_any_ternary_op_result_t<           \
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


/** Defines a free/non-member operator overload for unary operator \a op_name
    that produces an expression instantiated from the \a expr_template
    expression template.

    The \a x parameter to the defined operator overload may be any type that
    is \b not an expression and for which \code
    udt_trait<std::remove_cv_t<std::remove_reference_t<T>>>::value \endcode is
    true.  The parameter is wrapped in a terminal expression.

    Example:
    \snippet user_macros_snippets.cpp USER_UDT_UNARY_OPERATOR

    \param op_name The operator to be overloaded; this must be one of the \b
    unary enumerators in <code>expr_kind</code>, without the
    <code>expr_kind::</code> qualification.

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.

    \param udt_trait A trait template to use to constrain which types are
    accepted as template parameters to the defined operator overload.
*/
#define BOOST_YAP_USER_UDT_UNARY_OPERATOR(op_name, expr_template, udt_trait) \
    template <typename T>                                               \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)((T && x))            \
        -> ::boost::yap::detail::udt_unary_op_result_t<                 \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                           \
            T,                                                          \
            udt_trait                                                   \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::udt_unary_op_result< \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                           \
            T,                                                          \
            udt_trait                                                   \
        >;                                                              \
        using x_type = typename result_types::x_type;                   \
        using tuple_type = ::boost::hana::tuple<x_type>;                \
        return {tuple_type{x_type{static_cast<T &&>(x)}}};              \
    }


/** Defines a free/non-member operator overload for binary operator \a op_name
    that produces an expression instantiated from the \a expr_template
    expression template.

    The \a lhs parameter to the defined operator overload may be any type that
    is \b not an expression and for which \code
    t_udt_trait<std::remove_cv_t<std::remove_reference_t<T>>>::value \endcode is
    true.  The parameter is wrapped in a terminal expression.

    The \a rhs parameter to the defined operator overload may be any type that
    is \b not an expression and for which \code
    u_udt_trait<std::remove_cv_t<std::remove_reference_t<U>>>::value \endcode is
    true.  The parameter is wrapped in a terminal expression.

    Example:
    \snippet user_macros_snippets.cpp USER_UDT_UDT_BINARY_OPERATOR

    \param op_name The operator to be overloaded; this must be one of the \b
    binary enumerators in <code>expr_kind</code>, without the
    <code>expr_kind::</code> qualification.

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.

    \param t_udt_trait A trait template to use to constrain which types are
    accepted as \a T template parameters to the defined operator overload.

    \param u_udt_trait A trait template to use to constrain which types are
    accepted as \a U template parameters to the defined operator overload.
*/
#define BOOST_YAP_USER_UDT_UDT_BINARY_OPERATOR(op_name, expr_template, t_udt_trait, u_udt_trait) \
    template <typename T, typename U>                                   \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (T && lhs, U && rhs) \
        -> ::boost::yap::detail::udt_udt_binary_op_result_t<            \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                           \
            T,                                                          \
            U,                                                          \
            t_udt_trait,                                                \
            u_udt_trait                                                 \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::udt_udt_binary_op_result< \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                           \
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
    }


/** Defines a free/non-member operator overload for binary operator \a op_name
    that produces an expression instantiated from the \a expr_template
    expression template.

    The \a lhs and \a rhs parameters to the defined operator overload may be any types that
    is \b not expressions.  Each parameter is wrapped in a terminal expression.

    At least one of the parameters to the defined operator overload must be a
    type \c T for which \code
    udt_trait<std::remove_cv_t<std::remove_reference_t<T>>>::value \endcode is
    true.

    Example:
    \snippet user_macros_snippets.cpp USER_UDT_ANY_BINARY_OPERATOR

    \param op_name The operator to be overloaded; this must be one of the \b
    binary enumerators in <code>expr_kind</code>, without the
    <code>expr_kind::</code> qualification.

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.

    \param udt_trait A trait template to use to constrain which types are
    accepted as template parameters to the defined operator overload.  
*/
#define BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(op_name, expr_template, udt_trait) \
    template <typename T, typename U>                                   \
    auto operator BOOST_YAP_INDIRECT_CALL(op_name)() (T && lhs, U && rhs) \
        -> ::boost::yap::detail::udt_any_binary_op_result_t<            \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                           \
            T,                                                          \
            U,                                                          \
            udt_trait                                                   \
        >                                                               \
    {                                                                   \
        using result_types = ::boost::yap::detail::udt_any_binary_op_result< \
            expr_template,                                              \
            ::boost::yap::expr_kind::op_name,                           \
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
    }


/** Defines user defined literal template that creates literal placeholders
    instantiated from the \a expr_template expression template.  It is
    recommended that you put this in its own namespace.

    \param expr_template The expression template to use to instantiate the
    result expression.  \a expr_template must be an \ref
    ExpressionTemplate.
*/
#define BOOST_YAP_USER_LITERAL_PLACEHOLDER_OPERATOR(expr_template)      \
template <char ...c>                                                    \
constexpr auto operator"" _p ()                                         \
{                                                                       \
    using i = ::boost::hana::llong<                                     \
        ::boost::hana::ic_detail::parse<sizeof...(c)>({c...})           \
    >;                                                                  \
    static_assert(1 <= i::value, "Placeholders must be >= 1.");         \
    return expr_template<                                               \
        ::boost::yap::expr_kind::terminal,                              \
        ::boost::hana::tuple< ::boost::yap::placeholder<i::value>>      \
    >{};                                                                \
}

#endif
