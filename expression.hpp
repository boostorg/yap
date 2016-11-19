#ifndef BOOST_PROTO17_EXPRESSION_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_HPP_INCLUDED

#include "expression_fwd.hpp"
#include "detail/expression.hpp"

#include <boost/hana/tuple.hpp>
#include <boost/hana/size.hpp>
#include <boost/hana/comparing.hpp>


namespace boost::proto17 {

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T &&... args);

        struct eval_expression_as_fn
        {
            template <typename R, typename E, typename ...T>
            constexpr decltype(auto) operator() (E const & expr, hana::basic_type<R> rtype, T &&... args) const
            { return eval_expression_as(expr, rtype, static_cast<T &&>(args)...); }
        };

    }

    using adl_detail::eval_expression_as_fn;

    inline namespace function_objects {

        inline constexpr eval_expression_as_fn eval_expression_as{};

    }

    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr const & expr, T && ...t);

    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr const & expr, T && ...t);

    template <typename Expr, typename Transform>
    auto transform (Expr && expr, Transform && transform);

    template <expr_kind Kind, typename ...T>
    struct expression
    {
        using this_type = expression<Kind, T...>;
        using tuple_type = hana::tuple<T...>;

        static const expr_kind kind = Kind;

        expression (T && ... t) :
            elements (static_cast<T &&>(t)...)
        {}

        expression (hana::tuple<T...> && rhs) :
            elements (std::move(rhs))
        {}

        tuple_type elements;

#ifdef BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
        template <typename R>
        operator R ()
        { return eval_expression_as(*this, hana::basic_type<R>{}); }
#endif

        decltype(auto) value () const &
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 1UL,
                "value() is only defined for unary expressions."
            );
            return elements[0_c];
        }

        decltype(auto) value () &
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 1UL,
                "value() is only defined for unary expressions."
            );
            return elements[0_c];
        }

        decltype(auto) value () &&
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 1UL,
                "value() is only defined for unary expressions."
            );
            return static_cast<tuple_type &&>(elements)[0_c];
        }

        decltype(auto) left () const &
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 2UL,
                "left() and right() are only defined for binary expressions."
            );
            return elements[0_c];
        }

        decltype(auto) left () &
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 2UL,
                "left() and right() are only defined for binary expressions."
            );
            return elements[0_c];
        }

        decltype(auto) left () &&
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 2UL,
                "left() and right() are only defined for binary expressions."
            );
            return static_cast<tuple_type &&>(elements)[0_c];
        }

        decltype(auto) right () const &
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 2UL,
                "left() and right() are only defined for binary expressions."
            );
            return elements[1_c];
        }

        decltype(auto) right () &
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 2UL,
                "left() and right() are only defined for binary expressions."
            );
            return elements[1_c];
        }

        decltype(auto) right () &&
        {
            using namespace hana::literals;
            static_assert(
                decltype(hana::size(elements))::value == 2UL,
                "left() and right() are only defined for binary expressions."
            );
            return static_cast<tuple_type &&>(elements)[1_c];
        }

#define BOOST_PROTO17_UNARY_MEMBER_OPERATOR(op, op_name)                \
        auto operator op const &                                        \
        {                                                               \
            return expression<expr_kind::op_name, this_type>{           \
                hana::tuple<this_type>{*this}                           \
            };                                                          \
        }                                                               \
        auto operator op &&                                             \
        {                                                               \
            return expression<expr_kind::op_name, this_type>{           \
                hana::tuple<this_type>{std::move(*this)}                \
            };                                                          \
        }

        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(+(), unary_plus) // +
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(-(), negate) // -
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(*(), dereference) // *
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(~(), complement) // ~
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(&(), address_of) // &
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(!(), logical_not) // !
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(++(), pre_inc) // ++
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(--(), pre_dec) // --
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(++(int), post_inc) // ++(int)
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(--(int), post_dec) // --(int)

#undef BOOST_PROTO17_UNARY_MEMBER_OPERATOR

#define BOOST_PROTO17_BINARY_MEMBER_OPERATOR(op, op_name)               \
        template <typename U>                                           \
        auto operator op (U && rhs) const &                             \
        {                                                               \
            using rhs_type = detail::operand_type_t<U>;                 \
            return expression<expr_kind::op_name, this_type, rhs_type>{ \
                hana::tuple<this_type, rhs_type>{                       \
                    *this,                                              \
                    static_cast<U &&>(rhs)                              \
                }                                                       \
            };                                                          \
        }                                                               \
        template <typename U>                                           \
        auto operator op (U && rhs) &&                                  \
        {                                                               \
            using rhs_type = detail::operand_type_t<U>;                 \
            return expression<expr_kind::op_name, this_type, rhs_type>{ \
                hana::tuple<this_type, rhs_type>{                       \
                    std::move(*this),                                   \
                    static_cast<U &&>(rhs)                              \
                }                                                       \
            };                                                          \
        }

        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(<<, shift_left) // <<
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(>>, shift_right) // >>
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(*, multiplies) // *
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(/, divides) // /
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(%, modulus) // %
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(+, plus) // +
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(-, minus) // -
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(<, less) // <
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(>, greater) // >
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(<=, less_equal) // <=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(>=, greater_equal) // >=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(==, equal_to) // ==
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(!=, not_equal_to) // !=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(||, logical_or) // ||
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(&&, logical_and) // &&
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(&, bitwise_and) // &
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(|, bitwise_or) // |
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(^, bitwise_xor) // ^

        template <typename U>
        auto operator, (U && rhs) const &
        {
            using rhs_type = detail::operand_type_t<U>;
            return expression<expr_kind::comma, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{*this, static_cast<U &&>(rhs)}
            };
        }
        template <typename U>
        auto operator, (U && rhs) &&
        {
            using rhs_type = detail::operand_type_t<U>;
            return expression<expr_kind::comma, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{std::move(*this), static_cast<U &&>(rhs)}
            };
        }

        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(->*, mem_ptr) // ->*
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(=, assign) // =
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(<<=, shift_left_assign) // <<=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(>>=, shift_right_assign) // >>=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(*=, multiplies_assign) // *=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(/=, divides_assign) // /=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(%=, modulus_assign) // %=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(+=, plus_assign) // +=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(-=, minus_assign) // -=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(&=, bitwise_and_assign) // &=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(|=, bitwise_or_assign) // |=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(^=, bitwise_xor_assign) // ^=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR([], subscript) // []

#undef BOOST_PROTO17_BINARY_MEMBER_OPERATOR

        template <typename ...U>
        auto operator() (U && ...u) const &
        {
            using tuple_type = hana::tuple<this_type, detail::operand_type_t<U>...>;
            return detail::make_call_expression<tuple_type>(*this, static_cast<U &&>(u)...);
        }
        template <typename ...U>
        auto operator() (U && ...u) &&
        {
            using tuple_type = hana::tuple<this_type, detail::operand_type_t<U>...>;
            return detail::make_call_expression<tuple_type>(std::move(*this), static_cast<U &&>(u)...);
        }
    };

    template <typename Expr>
    decltype(auto) value (Expr const & expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 1UL,
            "value() is only defined for unary expressions."
        );
        return expr.elements[0_c];
    }

    template <typename Expr>
    decltype(auto) value (Expr & expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 1UL,
            "value() is only defined for unary expressions."
        );
        return expr.elements[0_c];
    }

    template <typename Expr>
    decltype(auto) value (std::remove_reference_t<Expr> && expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 1UL,
            "value() is only defined for unary expressions."
        );
        return static_cast<decltype(expr.elements) &&>(expr.elements)[0_c];
    }

    template <typename Expr>
    decltype(auto) left (Expr const & expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 2UL,
            "left() and right() are only defined for binary expressions."
        );
        return expr.elements[0_c];
    }

    template <typename Expr>
    decltype(auto) left (Expr & expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 2UL,
            "left() and right() are only defined for binary expressions."
        );
        return expr.elements[0_c];
    }

    template <typename Expr>
    decltype(auto) left (std::remove_reference_t<Expr> && expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 2UL,
            "left() and right() are only defined for binary expressions."
        );
        return static_cast<decltype(expr.elements) &&>(expr.elements)[0_c];
    }

    template <typename Expr>
    decltype(auto) right (Expr const & expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 2UL,
            "left() and right() are only defined for binary expressions."
        );
        return expr.elements[1_c];
    }

    template <typename Expr>
    decltype(auto) right (Expr & expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 2UL,
            "left() and right() are only defined for binary expressions."
        );
        return expr.elements[1_c];
    }

    template <typename Expr>
    decltype(auto) right (std::remove_reference_t<Expr> && expr)
    {
        using namespace hana::literals;
        static_assert(
            decltype(hana::size(expr.elements))::value == 2UL,
            "left() and right() are only defined for binary expressions."
        );
        return static_cast<decltype(expr.elements) &&>(expr.elements)[1_c];
    }

    namespace detail {

        template <expr_kind OpKind, typename T, typename U,
                  bool Expr = detail::is_expr<remove_cv_ref_t<T>>::value>
        struct binary_op_result
        {
            using lhs_type = detail::operand_type_t<T>;
            using rhs_type = U;
            using type = expression<OpKind, lhs_type, rhs_type>;
        };

        template <expr_kind OpKind, typename T, typename U>
        struct binary_op_result<OpKind, T, U, true>
        {};

    }

#define BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(op, op_name)           \
    template <typename T, expr_kind Kind, typename ...U>                \
    auto operator op (T && lhs, expression<Kind, U...> const & rhs)     \
        -> typename detail::binary_op_result<expr_kind::op_name, T, expression<Kind, U...>>::type \
    {                                                                   \
        using lhs_type = detail::operand_type_t<T>;                     \
        using rhs_type = expression<Kind, U...>;                        \
        return {                                                        \
            hana::tuple<lhs_type, rhs_type>{static_cast<T &&>(lhs), rhs} \
        };                                                              \
    }                                                                   \
    template <typename T, expr_kind Kind, typename ...U>                \
    auto operator op (T && lhs, expression<Kind, U...> && rhs)          \
        -> typename detail::binary_op_result<expr_kind::op_name, T, expression<Kind, U...>>::type \
    {                                                                   \
        using lhs_type = detail::operand_type_t<T>;                     \
        using rhs_type = expression<Kind, U...>;                        \
        return {                                                        \
            hana::tuple<lhs_type, rhs_type>{static_cast<T &&>(lhs), static_cast<rhs_type &&>(rhs)} \
        };                                                              \
    }

    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(<<, shift_left) // <<
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(>>, shift_right) // >>
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(*, multiplies) // *
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(/, divides) // /
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(%, modulus) // %
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(+, plus) // +
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(-, minus) // -
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(<, less) // <
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(>, greater) // >
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(<=, less_equal) // <=
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(>=, greater_equal) // >=
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(==, equal_to) // ==
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(!=, not_equal_to) // !=
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(||, logical_or) // ||
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(&&, logical_and) // &&
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(&, bitwise_and) // &
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(|, bitwise_or) // |
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(^, bitwise_xor) // ^

#undef BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR

    template <expr_kind Kind, typename ...T>
    auto make_expression (T &&... t)
    {
        return expression<Kind, T...>{
            hana::tuple<T...>{static_cast<T &&>(t)...}
        };
    }

    template <typename T>
    auto make_terminal (T && t)
    {
        return expression<expr_kind::terminal, T>{
            hana::tuple<T>{static_cast<T &&>(t)}
        };
    }

    template <expr_kind Kind, typename ...T>
    struct expression_function
    {
        template <typename ...U>
        decltype(auto) operator() (U &&... u)
        { return evaluate(expr, static_cast<U &&>(u)...); }

        expression<Kind, T...> expr;
    };

    template <expr_kind Kind, typename ...T>
    auto make_expression_function (expression<Kind, T...> && expr)
    { return expression_function<Kind, T...>{std::move(expr)}; }

}

#include "detail/default_eval.hpp"

namespace boost::proto17 {

    // TODO: static assert/SFINAE sizeof...(T) >= highest-indexed placeholder
    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr const & expr, T && ...t)
    { return detail::default_eval_expr(expr, static_cast<T &&>(t)...); }

    // TODO: static assert/SFINAE sizeof...(T) >= highest-indexed placeholder
    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr const & expr, T && ...t)
    { return eval_expression_as(expr, hana::basic_type<R>{}, static_cast<T &&>(t)...); }

    template <typename Expr, typename Transform>
    auto transform (Expr && expr, Transform && transform)
    {
        return detail::default_transform_expression<Expr, Transform>{}(
            static_cast<Expr &&>(expr),
            static_cast<Transform &&>(transform)
        );
    }

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T &&... args)
        { return static_cast<R>(detail::default_eval_expr(expr, static_cast<T &&>(args)...)); }

    }

}

#endif
