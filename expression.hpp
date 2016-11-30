#ifndef BOOST_PROTO17_EXPRESSION_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_HPP_INCLUDED

#include "expression_fwd.hpp"
#include "user_macros.hpp"
#include "detail/expression.hpp"

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

    inline char const * op_string (expr_kind kind)
    {
        switch (kind) {
        case expr_kind::unary_plus: return "+";
        case expr_kind::negate: return "-";
        case expr_kind::dereference: return "*";
        case expr_kind::complement: return "~";
        case expr_kind::address_of: return "&";
        case expr_kind::logical_not: return "!";
        case expr_kind::pre_inc: return "++";
        case expr_kind::pre_dec: return "--";
        case expr_kind::post_inc: return "++(int)";
        case expr_kind::post_dec: return "--(int)";

        case expr_kind::shift_left: return "<<";
        case expr_kind::shift_right: return ">>";
        case expr_kind::multiplies: return "*";
        case expr_kind::divides: return "/";
        case expr_kind::modulus: return "%";
        case expr_kind::plus: return "+";
        case expr_kind::minus: return "-";
        case expr_kind::less: return "<";
        case expr_kind::greater: return ">";
        case expr_kind::less_equal: return "<=";
        case expr_kind::greater_equal: return ">=";
        case expr_kind::equal_to: return "==";
        case expr_kind::not_equal_to: return "!=";
        case expr_kind::logical_or: return "||";
        case expr_kind::logical_and: return "&&";
        case expr_kind::bitwise_and: return "&";
        case expr_kind::bitwise_or: return "|";
        case expr_kind::bitwise_xor: return "^";
        case expr_kind::comma: return ",";
        case expr_kind::mem_ptr: return "->*";
        case expr_kind::assign: return "=";
        case expr_kind::shift_left_assign: return "<<=";
        case expr_kind::shift_right_assign: return ">>=";
        case expr_kind::multiplies_assign: return "*=";
        case expr_kind::divides_assign: return "/=";
        case expr_kind::modulus_assign: return "%=";
        case expr_kind::plus_assign: return "+=";
        case expr_kind::minus_assign: return "-=";
        case expr_kind::bitwise_and_assign: return "&=";
        case expr_kind::bitwise_or_assign: return "|=";
        case expr_kind::bitwise_xor_assign: return "^=";
        case expr_kind::subscript: return "[]";

        case expr_kind::call: return "()";

        default: return "** ERROR: UNKNOWN OPERATOR! **";
        }
    }

    template <expr_kind Kind, typename Tuple>
    struct expression
    {
        using this_type = expression<Kind, Tuple>;
        using tuple_type = Tuple;

        static const expr_kind kind = Kind;

        expression () {}

        expression (tuple_type && rhs) :
            elements (std::move(rhs))
        {}

        tuple_type elements;

#ifdef BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
        template <typename R>
        operator R ()
        { return eval_expression_as(*this, hana::basic_type<R>{}); }
#endif

        decltype(auto) value () const &
        { return ::boost::proto17::value(*this); }

        decltype(auto) value () &
        { return ::boost::proto17::value(*this); }

        decltype(auto) value () &&
        { return ::boost::proto17::value(std::move(*this)); }

        decltype(auto) left () const &
        { return ::boost::proto17::left(*this); }

        decltype(auto) left () &
        { return ::boost::proto17::left(*this); }

        decltype(auto) left () &&
        { return ::boost::proto17::left(std::move(*this)); }

        decltype(auto) right () const &
        { return ::boost::proto17::right(*this); }

        decltype(auto) right () &
        { return ::boost::proto17::right(*this); }

        decltype(auto) right () &&
        { return ::boost::proto17::right(std::move(*this)); }

#define BOOST_PROTO17_UNARY_MEMBER_OPERATOR(op_name)                    \
        BOOST_PROTO17_USER_UNARY_OPERATOR_MEMBER(op_name, this_type, ::boost::proto17::expression)

        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(unary_plus) // +
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(negate) // -
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(dereference) // *
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(complement) // ~
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(address_of) // &
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(logical_not) // !
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(pre_inc) // ++
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(pre_dec) // --
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(post_inc) // ++(int)
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(post_dec) // --(int)

#undef BOOST_PROTO17_UNARY_MEMBER_OPERATOR

        // TODO: Add test coverage for all the operators (with all three qual
        // types), for expression and terminal.  Don't forget the free
        // operators.
#define BOOST_PROTO17_BINARY_MEMBER_OPERATOR(op_name)                   \
        BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(op_name, this_type, ::boost::proto17::expression)

        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(shift_left) // <<
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(shift_right) // >>
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(multiplies) // *
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(divides) // /
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(modulus) // %
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(plus) // +
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(minus) // -
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(less) // <
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(greater) // >
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(less_equal) // <=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(greater_equal) // >=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(equal_to) // ==
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(not_equal_to) // !=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(logical_or) // ||
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(logical_and) // &&
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_and) // &
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_or) // |
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_xor) // ^
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(comma) // ,
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(mem_ptr) // ->*
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(assign) // =
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(shift_left_assign) // <<=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(shift_right_assign) // >>=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(multiplies_assign) // *=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(divides_assign) // /=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(modulus_assign) // %=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(plus_assign) // +=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(minus_assign) // -=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_and_assign) // &=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_or_assign) // |=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_xor_assign) // ^=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(subscript) // []

#undef BOOST_PROTO17_BINARY_MEMBER_OPERATOR

        BOOST_PROTO17_USER_MEMBER_CALL_OPERATOR(this_type, ::boost::proto17::expression)
    };

    template <typename T>
    struct expression<expr_kind::terminal, hana::tuple<T>>
    {
        using this_type = expression<expr_kind::terminal, hana::tuple<T>>;
        using tuple_type = hana::tuple<T>;

        static const expr_kind kind = expr_kind::terminal;

        expression () {}

        expression (T && t) :
            elements (static_cast<T &&>(t))
        {}

        expression (hana::tuple<T> && rhs) :
            elements (std::move(rhs))
        {}

        tuple_type elements;

#ifdef BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
        template <typename R>
        operator R ()
        { return eval_expression_as(*this, hana::basic_type<R>{}); }
#endif

        decltype(auto) value () const &
        { return ::boost::proto17::value(*this); }

        decltype(auto) value () &
        { return ::boost::proto17::value(*this); }

        decltype(auto) value () &&
        { return ::boost::proto17::value(std::move(*this)); }

#define BOOST_PROTO17_UNARY_MEMBER_OPERATOR(op_name)                    \
        BOOST_PROTO17_USER_UNARY_OPERATOR_MEMBER(op_name, this_type, ::boost::proto17::expression)

        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(unary_plus) // +
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(negate) // -
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(dereference) // *
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(complement) // ~
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(address_of) // &
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(logical_not) // !
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(pre_inc) // ++
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(pre_dec) // --
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(post_inc) // ++(int)
        BOOST_PROTO17_UNARY_MEMBER_OPERATOR(post_dec) // --(int)

#undef BOOST_PROTO17_UNARY_MEMBER_OPERATOR

#define BOOST_PROTO17_BINARY_MEMBER_OPERATOR(op_name)                   \
        BOOST_PROTO17_USER_BINARY_OPERATOR_MEMBER(op_name, this_type, ::boost::proto17::expression)

        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(shift_left) // <<
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(shift_right) // >>
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(multiplies) // *
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(divides) // /
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(modulus) // %
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(plus) // +
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(minus) // -
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(less) // <
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(greater) // >
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(less_equal) // <=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(greater_equal) // >=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(equal_to) // ==
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(not_equal_to) // !=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(logical_or) // ||
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(logical_and) // &&
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_and) // &
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_or) // |
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_xor) // ^
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(comma) // ,
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(mem_ptr) // ->*
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(assign) // =
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(shift_left_assign) // <<=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(shift_right_assign) // >>=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(multiplies_assign) // *=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(divides_assign) // /=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(modulus_assign) // %=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(plus_assign) // +=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(minus_assign) // -=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_and_assign) // &=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_or_assign) // |=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(bitwise_xor_assign) // ^=
        BOOST_PROTO17_BINARY_MEMBER_OPERATOR(subscript) // []

#undef BOOST_PROTO17_BINARY_MEMBER_OPERATOR

        BOOST_PROTO17_USER_MEMBER_CALL_OPERATOR(this_type, ::boost::proto17::expression)
    };

    template <long long I>
    using placeholder = expression<expr_kind::placeholder, hana::tuple<hana::llong<I>>>;

    // TODO: Test all the cases here.

    template <typename T>
    decltype(auto) deref (T && x)
    {
        if constexpr (detail::is_expr<T>::value) {
            if constexpr (detail::remove_cv_ref_t<T>::kind == expr_kind::expr_ref) {
                return ::boost::proto17::value(::boost::proto17::value(static_cast<T &&>(x)));
            } else {
                return static_cast<T &&>(x);
            }
        } else {
            return static_cast<T &&>(x);
        }
    }

    template <typename Expr>
    decltype(auto) value (Expr && expr)
    {
        using namespace hana::literals;
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        static_assert(
            detail::arity_of<kind>() == detail::expr_arity::one,
            "value() is only defined for unary expressions."
        );
        if constexpr (kind == expr_kind::expr_ref) {
            if constexpr (std::is_rvalue_reference<Expr>{} && !std::is_const<std::remove_reference_t<Expr>>{}) {
                return std::move(*expr.elements[0_c]);
            } else {
                return *expr.elements[0_c];
            }
        } else {
            if constexpr (std::is_lvalue_reference<Expr>{}) {
                return expr.elements[0_c];
            } else {
                return std::move(expr.elements[0_c]);
            }
        }
    }

    template <typename Expr>
    decltype(auto) left (Expr && expr)
    {
        using namespace hana::literals;
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind == expr_kind::expr_ref) {
            return left(::boost::proto17::value(static_cast<Expr &&>(expr)));
        } else {
            static_assert(
                detail::arity_of<kind>() == detail::expr_arity::two,
                "left() is only defined for binary expressions."
            );
            if constexpr (std::is_lvalue_reference<Expr>{}) {
                return expr.elements[0_c];
            } else {
                return std::move(expr.elements[0_c]);
            }
        }
    }

    template <typename Expr>
    decltype(auto) right (Expr const & expr)
    {
        using namespace hana::literals;
        if constexpr (Expr::kind == expr_kind::expr_ref) {
            return right(*expr.elements[1_c]);
        } else {
            static_assert(
                detail::arity_of<detail::remove_cv_ref_t<Expr>::kind>() == detail::expr_arity::two,
                "right() is only defined for binary expressions."
            );
            return expr.elements[1_c];
        }
    }

    template <typename Expr>
    decltype(auto) right (Expr & expr)
    {
        using namespace hana::literals;
        if constexpr (Expr::kind == expr_kind::expr_ref) {
            return right(*expr.elements[1_c]);
        } else {
            static_assert(
                detail::arity_of<detail::remove_cv_ref_t<Expr>::kind>() == detail::expr_arity::two,
                "left() and right() are only defined for binary expressions."
            );
            return expr.elements[1_c];
        }
    }

    template <typename Expr>
    decltype(auto) right (std::remove_reference_t<Expr> && expr)
    {
        using namespace hana::literals;
        if constexpr (Expr::kind == expr_kind::expr_ref) {
            return right(std::move(*expr.elements[1_c]));
        } else {
            static_assert(
                detail::arity_of<detail::remove_cv_ref_t<Expr>::kind>() == detail::expr_arity::two,
                "left() and right() are only defined for binary expressions."
            );
            return std::move(expr.elements)[1_c];
        }
    }

#define BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(op_name)               \
    BOOST_PROTO17_USER_NON_MEMBER_BINARY_OPERATOR(op_name, expression)

    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(shift_left) // <<
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(shift_right) // >>
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(multiplies) // *
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(divides) // /
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(modulus) // %
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(plus) // +
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(minus) // -
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(less) // <
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(greater) // >
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(less_equal) // <=
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(greater_equal) // >=
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(equal_to) // ==
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(not_equal_to) // !=
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(logical_or) // ||
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(logical_and) // &&
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(bitwise_and) // &
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(bitwise_or) // |
    BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR(bitwise_xor) // ^

#undef BOOST_PROTO17_BINARY_NON_MEMBER_OPERATOR

    template <expr_kind Kind, typename ...T>
    auto make_expression (T &&... t)
    {
        using tuple_type = hana::tuple<detail::operand_type_t<expression, T>...>;
        return expression<Kind, tuple_type>{
            tuple_type{
                detail::make_operand<detail::operand_type_t<expression, T>>{}(static_cast<T &&>(t))...
            }
        };
    }

    template <template <expr_kind, class> class ExprTemplate, expr_kind Kind, typename ...T>
    auto make_expression (T &&... t)
    {
        using tuple_type = hana::tuple<detail::operand_type_t<ExprTemplate, T>...>;
        return ExprTemplate<Kind, tuple_type>{
            tuple_type{
                detail::make_operand<detail::operand_type_t<ExprTemplate, T>>{}(static_cast<T &&>(t))...
            }
        };
    }

    template <typename T>
    auto make_terminal (T && t)
    {
        static_assert(!detail::is_expr<T>::value);
        using result_type = detail::operand_type_t<expression, T>;
        using tuple_type = typename result_type::tuple_type;
        return result_type{tuple_type{static_cast<T &&>(t)}};
    }

    template <template <expr_kind, class> class ExprTemplate, typename T>
    auto make_terminal (T && t)
    {
        static_assert(!detail::is_expr<T>::value);
        using result_type = detail::operand_type_t<ExprTemplate, T>;
        using tuple_type = decltype(std::declval<result_type>().elements);
        return result_type{tuple_type{static_cast<T &&>(t)}};
    }

    template <typename T>
    decltype(auto) as_expr (T && t)
    {
        if constexpr (detail::is_expr<T>::value) {
            return static_cast<T &&>(t);
        } else {
            return make_terminal(static_cast<T &&>(t));
        }
    }

    template <template <expr_kind, class> class ExprTemplate, typename T>
    decltype(auto) as_expr (T && t)
    {
        if constexpr (detail::is_expr<T>::value) {
            return static_cast<T &&>(t);
        } else {
            return make_terminal<ExprTemplate>(static_cast<T &&>(t));
        }
    }

    template <typename Expr>
    struct expression_function
    {
        template <typename ...U>
        decltype(auto) operator() (U &&... u)
        { return evaluate(expr, static_cast<U &&>(u)...); }

        Expr expr;
    };

    template <typename Expr>
    auto make_expression_function (Expr && expr)
    { return expression_function<Expr>{std::move(expr)}; }

}

#include "detail/default_eval.hpp"

namespace boost::proto17 {

    // TODO: static_assert is_expr in these?  In others?

    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr const & expr, T && ...t)
    { return detail::default_eval_expr(expr, static_cast<T &&>(t)...); }

    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr const & expr, T && ...t)
    { return eval_expression_as(expr, hana::basic_type<R>{}, static_cast<T &&>(t)...); }

    template <typename Expr, typename Transform>
    decltype(auto) transform (Expr && expr, Transform && transform)
    {
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        return detail::default_transform_expression<Expr, Transform, detail::arity_of<kind>()>{}(
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
