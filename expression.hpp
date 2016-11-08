#ifndef BOOST_PROTO17_EXPRESSION_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_HPP_INCLUDED

#include "expression_fwd.hpp"
#include "detail/expression.hpp"
#include "detail/default_eval.hpp"

#include <boost/hana/tuple.hpp>


namespace boost::proto17 {

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T && ...t)
        { return static_cast<R>(detail::default_eval_expr(expr, hana::make_tuple(static_cast<T &&>(t)...))); }

        struct eval_expression_as_fn
        {
            template <typename R, typename E, typename ...T>
            constexpr decltype(auto) operator() (E const & expr, hana::basic_type<R> rtype, T && ...t) const
            { return eval_expression_as(expr, rtype, static_cast<T &&>(t)...); }
        };

    }

    using adl_detail::eval_expression_as_fn;

    inline namespace function_objects {

        inline constexpr eval_expression_as_fn eval_expression_as{};

    }

    // TODO: static assert/SFINAE sizeof...(T) >= highest-indexed placeholder + 1
    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr const & expr, T && ...t)
    { return detail::default_eval_expr(expr, hana::make_tuple(static_cast<T &&>(t)...)); }

    // TODO: static assert/SFINAE sizeof...(T) >= highest-indexed placeholder + 1
    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr const & expr, T && ...t)
    { return eval_expression_as(expr, hana::basic_type<R>{}, static_cast<T &&>(t)...); }

    template <expr_kind Kind, typename ...T>
    struct expression
    {
        using this_type = expression<Kind, T...>;
        using tuple_type = hana::tuple<T...>;

        static const expr_kind kind = Kind;

        expression (T && ... t) :
            elements (static_cast<T &&>(t)...)
        {}

        expression (hana::tuple<T...> const & rhs) :
            elements (rhs)
        {}

        expression (hana::tuple<T...> && rhs) :
            elements (std::move(rhs))
        {}

        expression & operator= (hana::tuple<T...> const & rhs)
        { elements = rhs.elements; }

        expression & operator= (hana::tuple<T...> && rhs)
        { elements = std::move(rhs.elements); }

        tuple_type elements;

        template <typename R>
        operator R ()
        { return eval_expression_as(*this, hana::basic_type<R>{}); }

#define BOOST_PROTO17_UNARY_MEMBER_OPERATOR(op, op_name)                \
        decltype(auto) operator op const &                              \
        {                                                               \
            return expression<expr_kind::op_name, this_type>{           \
                hana::tuple<this_type>{*this}                           \
            };                                                          \
        }                                                               \
        decltype(auto) operator op &&                                   \
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
        decltype(auto) operator op (U && rhs) const &                   \
        {                                                               \
            using rhs_type = typename detail::rhs_type<U>::type;        \
            return expression<expr_kind::op_name, this_type, rhs_type>{ \
                hana::tuple<this_type, rhs_type>{                       \
                    *this,                                              \
                    static_cast<U &&>(rhs)                              \
                }                                                       \
            };                                                          \
        }                                                               \
        template <typename U>                                           \
        decltype(auto) operator op (U && rhs) &&                        \
        {                                                               \
            using rhs_type = typename detail::rhs_type<U>::type;        \
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
        decltype(auto) operator, (U && rhs) const &
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::comma, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{*this, static_cast<U &&>(rhs)}
            };
        }
        template <typename U>
        decltype(auto) operator, (U && rhs) &&
        {
            using rhs_type = typename detail::rhs_type<U>::type;
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

        template <typename ...U>
        decltype(auto) operator() (U && ...u) const &
        {
            using tuple_type = hana::tuple<this_type, typename detail::rhs_type<U>::type...>;
            return detail::make_call_expression<tuple_type>(*this, static_cast<U &&>(u)...);
        }
        template <typename ...U>
        decltype(auto) operator() (U && ...u) &&
        {
            using tuple_type = hana::tuple<this_type, typename detail::rhs_type<U>::type...>;
            return detail::make_call_expression<tuple_type>(std::move(*this), static_cast<U &&>(u)...);
        }
    };

    template <typename F>
    auto make_terminal (F && f)
    {
        return expression<expr_kind::terminal, F>{
            hana::tuple<F>{static_cast<F &&>(f)}
        };
    }

}

#endif
