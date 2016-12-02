#ifndef BOOST_YAP_EXPRESSION_HPP_INCLUDED
#define BOOST_YAP_EXPRESSION_HPP_INCLUDED

#include <boost/yap/expression_fwd.hpp>
#include <boost/yap/user_macros.hpp>
#include <boost/yap/detail/expression.hpp>

#include <boost/hana/size.hpp>
#include <boost/hana/comparing.hpp>


namespace boost::yap {

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

    /** Returns the <code>char const *</code> string for the spelling of the
        C++ operator associated with \a kind. */
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

        case expr_kind::if_else: return "?:";

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

#ifdef BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE
        template <typename R>
        operator R ()
        { return eval_expression_as(*this, hana::basic_type<R>{}); }
#endif

        decltype(auto) value () const &
        { return ::boost::yap::value(*this); }

        decltype(auto) value () &
        { return ::boost::yap::value(*this); }

        decltype(auto) value () &&
        { return ::boost::yap::value(std::move(*this)); }

        decltype(auto) left () const &
        { return ::boost::yap::left(*this); }

        decltype(auto) left () &
        { return ::boost::yap::left(*this); }

        decltype(auto) left () &&
        { return ::boost::yap::left(std::move(*this)); }

        decltype(auto) right () const &
        { return ::boost::yap::right(*this); }

        decltype(auto) right () &
        { return ::boost::yap::right(*this); }

        decltype(auto) right () &&
        { return ::boost::yap::right(std::move(*this)); }

#define BOOST_YAP_UNARY_MEMBER_OPERATOR(op_name)                    \
        BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(op_name, this_type, ::boost::yap::expression)

        BOOST_YAP_UNARY_MEMBER_OPERATOR(unary_plus) // +
        BOOST_YAP_UNARY_MEMBER_OPERATOR(negate) // -
        BOOST_YAP_UNARY_MEMBER_OPERATOR(dereference) // *
        BOOST_YAP_UNARY_MEMBER_OPERATOR(complement) // ~
        BOOST_YAP_UNARY_MEMBER_OPERATOR(address_of) // &
        BOOST_YAP_UNARY_MEMBER_OPERATOR(logical_not) // !
        BOOST_YAP_UNARY_MEMBER_OPERATOR(pre_inc) // ++
        BOOST_YAP_UNARY_MEMBER_OPERATOR(pre_dec) // --
        BOOST_YAP_UNARY_MEMBER_OPERATOR(post_inc) // ++(int)
        BOOST_YAP_UNARY_MEMBER_OPERATOR(post_dec) // --(int)

#undef BOOST_YAP_UNARY_MEMBER_OPERATOR

        // TODO: Add test coverage for all the operators (with all three qual
        // types), for expression and terminal.  Don't forget the free
        // operators.
#define BOOST_YAP_BINARY_MEMBER_OPERATOR(op_name)                   \
        BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(op_name, this_type, ::boost::yap::expression)

        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_left) // <<
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_right) // >>
        BOOST_YAP_BINARY_MEMBER_OPERATOR(multiplies) // *
        BOOST_YAP_BINARY_MEMBER_OPERATOR(divides) // /
        BOOST_YAP_BINARY_MEMBER_OPERATOR(modulus) // %
        BOOST_YAP_BINARY_MEMBER_OPERATOR(plus) // +
        BOOST_YAP_BINARY_MEMBER_OPERATOR(minus) // -
        BOOST_YAP_BINARY_MEMBER_OPERATOR(less) // <
        BOOST_YAP_BINARY_MEMBER_OPERATOR(greater) // >
        BOOST_YAP_BINARY_MEMBER_OPERATOR(less_equal) // <=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(greater_equal) // >=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(equal_to) // ==
        BOOST_YAP_BINARY_MEMBER_OPERATOR(not_equal_to) // !=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(logical_or) // ||
        BOOST_YAP_BINARY_MEMBER_OPERATOR(logical_and) // &&
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_and) // &
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_or) // |
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_xor) // ^
        BOOST_YAP_BINARY_MEMBER_OPERATOR(comma) // ,
        BOOST_YAP_BINARY_MEMBER_OPERATOR(mem_ptr) // ->*
        BOOST_YAP_BINARY_MEMBER_OPERATOR(assign) // =
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_left_assign) // <<=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_right_assign) // >>=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(multiplies_assign) // *=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(divides_assign) // /=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(modulus_assign) // %=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(plus_assign) // +=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(minus_assign) // -=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_and_assign) // &=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_or_assign) // |=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_xor_assign) // ^=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(subscript) // []

#undef BOOST_YAP_BINARY_MEMBER_OPERATOR

        BOOST_YAP_USER_MEMBER_CALL_OPERATOR(this_type, ::boost::yap::expression)
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

#ifdef BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE
        template <typename R>
        operator R ()
        { return eval_expression_as(*this, hana::basic_type<R>{}); }
#endif

        decltype(auto) value () const &
        { return ::boost::yap::value(*this); }

        decltype(auto) value () &
        { return ::boost::yap::value(*this); }

        decltype(auto) value () &&
        { return ::boost::yap::value(std::move(*this)); }

#define BOOST_YAP_UNARY_MEMBER_OPERATOR(op_name)                    \
        BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(op_name, this_type, ::boost::yap::expression)

        BOOST_YAP_UNARY_MEMBER_OPERATOR(unary_plus) // +
        BOOST_YAP_UNARY_MEMBER_OPERATOR(negate) // -
        BOOST_YAP_UNARY_MEMBER_OPERATOR(dereference) // *
        BOOST_YAP_UNARY_MEMBER_OPERATOR(complement) // ~
        BOOST_YAP_UNARY_MEMBER_OPERATOR(address_of) // &
        BOOST_YAP_UNARY_MEMBER_OPERATOR(logical_not) // !
        BOOST_YAP_UNARY_MEMBER_OPERATOR(pre_inc) // ++
        BOOST_YAP_UNARY_MEMBER_OPERATOR(pre_dec) // --
        BOOST_YAP_UNARY_MEMBER_OPERATOR(post_inc) // ++(int)
        BOOST_YAP_UNARY_MEMBER_OPERATOR(post_dec) // --(int)

#undef BOOST_YAP_UNARY_MEMBER_OPERATOR

#define BOOST_YAP_BINARY_MEMBER_OPERATOR(op_name)                   \
        BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(op_name, this_type, ::boost::yap::expression)

        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_left) // <<
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_right) // >>
        BOOST_YAP_BINARY_MEMBER_OPERATOR(multiplies) // *
        BOOST_YAP_BINARY_MEMBER_OPERATOR(divides) // /
        BOOST_YAP_BINARY_MEMBER_OPERATOR(modulus) // %
        BOOST_YAP_BINARY_MEMBER_OPERATOR(plus) // +
        BOOST_YAP_BINARY_MEMBER_OPERATOR(minus) // -
        BOOST_YAP_BINARY_MEMBER_OPERATOR(less) // <
        BOOST_YAP_BINARY_MEMBER_OPERATOR(greater) // >
        BOOST_YAP_BINARY_MEMBER_OPERATOR(less_equal) // <=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(greater_equal) // >=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(equal_to) // ==
        BOOST_YAP_BINARY_MEMBER_OPERATOR(not_equal_to) // !=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(logical_or) // ||
        BOOST_YAP_BINARY_MEMBER_OPERATOR(logical_and) // &&
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_and) // &
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_or) // |
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_xor) // ^
        BOOST_YAP_BINARY_MEMBER_OPERATOR(comma) // ,
        BOOST_YAP_BINARY_MEMBER_OPERATOR(mem_ptr) // ->*
        BOOST_YAP_BINARY_MEMBER_OPERATOR(assign) // =
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_left_assign) // <<=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_right_assign) // >>=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(multiplies_assign) // *=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(divides_assign) // /=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(modulus_assign) // %=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(plus_assign) // +=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(minus_assign) // -=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_and_assign) // &=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_or_assign) // |=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_xor_assign) // ^=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(subscript) // []

#undef BOOST_YAP_BINARY_MEMBER_OPERATOR

        BOOST_YAP_USER_MEMBER_CALL_OPERATOR(this_type, ::boost::yap::expression)
    };

    template <long long I>
    using placeholder = expression<expr_kind::placeholder, hana::tuple<hana::llong<I>>>;

    template <typename Expr>
    decltype(auto) deref (Expr && expr)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "deref() is only defined for expressions."
        );

        static_assert(
            detail::remove_cv_ref_t<Expr>::kind == expr_kind::expr_ref,
            "deref() is only defined for expr_ref-kind expressions."
        );

        using namespace hana::literals;
        if constexpr (
            std::is_rvalue_reference<Expr>{} &&
            !std::is_const<std::remove_reference_t<Expr>>{}
        ) {
            return std::move(*expr.elements[0_c]);
        } else {
            return *expr.elements[0_c];
        }
    }

    template <typename T>
    decltype(auto) value (T && x)
    {
        if constexpr (detail::is_expr<T>::value) {
            using namespace hana::literals;
            constexpr expr_kind kind = detail::remove_cv_ref_t<T>::kind;
            if constexpr (kind == expr_kind::expr_ref) {
                return ::boost::yap::value(::boost::yap::deref(static_cast<T &&>(x)));
            } else if constexpr (kind == expr_kind::terminal || kind == expr_kind::placeholder) {
                if constexpr (std::is_lvalue_reference<T>{}) {
                    return x.elements[0_c];
                } else {
                    return std::move(x.elements[0_c]);
                }
            } else {
                return static_cast<T &&>(x);
            }
        } else {
            return static_cast<T &&>(x);
        }
    }

    template <typename Expr>
    decltype(auto) left (Expr && expr)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "left() is only defined for expressions."
        );

        using namespace hana::literals;
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind == expr_kind::expr_ref) {
            return ::boost::yap::left(::boost::yap::deref(static_cast<Expr &&>(expr)));
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
    decltype(auto) right (Expr && expr)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "right() is only defined for expressions."
        );

        using namespace hana::literals;
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind == expr_kind::expr_ref) {
            return ::boost::yap::right(::boost::yap::deref(static_cast<Expr &&>(expr)));
        } else {
            static_assert(
                detail::arity_of<kind>() == detail::expr_arity::two,
                "right() is only defined for binary expressions."
            );
            if constexpr (std::is_lvalue_reference<Expr>{}) {
                return expr.elements[1_c];
            } else {
                return std::move(expr.elements[1_c]);
            }
        }
    }

    template <long long I, typename Expr>
    decltype(auto) argument (Expr && expr, hana::llong<I> i)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "argument() is only defined for expressions."
        );

        using namespace hana::literals;
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind == expr_kind::expr_ref) {
            return ::boost::yap::argument(::boost::yap::deref(static_cast<Expr &&>(expr)), i);
        } else {
            static_assert(
                detail::arity_of<kind>() == detail::expr_arity::n,
                "argument() is only defined for call expressions."
            );
            static_assert(
                0 <= I && I < decltype(hana::size(expr.elements))::value,
                "In argument(expr, I), I must be nonnegative, and less "
                "than hana::size(expr.elements)."
            );
            if constexpr (std::is_lvalue_reference<Expr>{}) {
                return expr.elements[i];
            } else {
                return std::move(expr.elements[i]);
            }
        }
    }

#define BOOST_YAP_BINARY_FREE_OPERATOR(op_name)                     \
    BOOST_YAP_USER_FREE_BINARY_OPERATOR(op_name, expression)

    BOOST_YAP_BINARY_FREE_OPERATOR(shift_left) // <<
    BOOST_YAP_BINARY_FREE_OPERATOR(shift_right) // >>
    BOOST_YAP_BINARY_FREE_OPERATOR(multiplies) // *
    BOOST_YAP_BINARY_FREE_OPERATOR(divides) // /
    BOOST_YAP_BINARY_FREE_OPERATOR(modulus) // %
    BOOST_YAP_BINARY_FREE_OPERATOR(plus) // +
    BOOST_YAP_BINARY_FREE_OPERATOR(minus) // -
    BOOST_YAP_BINARY_FREE_OPERATOR(less) // <
    BOOST_YAP_BINARY_FREE_OPERATOR(greater) // >
    BOOST_YAP_BINARY_FREE_OPERATOR(less_equal) // <=
    BOOST_YAP_BINARY_FREE_OPERATOR(greater_equal) // >=
    BOOST_YAP_BINARY_FREE_OPERATOR(equal_to) // ==
    BOOST_YAP_BINARY_FREE_OPERATOR(not_equal_to) // !=
    BOOST_YAP_BINARY_FREE_OPERATOR(logical_or) // ||
    BOOST_YAP_BINARY_FREE_OPERATOR(logical_and) // &&
    BOOST_YAP_BINARY_FREE_OPERATOR(bitwise_and) // &
    BOOST_YAP_BINARY_FREE_OPERATOR(bitwise_or) // |
    BOOST_YAP_BINARY_FREE_OPERATOR(bitwise_xor) // ^

#undef BOOST_YAP_BINARY_FREE_OPERATOR

    BOOST_YAP_USER_EXPR_IF_ELSE(::boost::yap::expression)

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

    template <expr_kind Kind, typename ...T>
    auto make_expression (T &&... t)
    { return make_expression<expression, Kind>(static_cast<T &&>(t)...); }

    template <template <expr_kind, class> class ExprTemplate, typename T>
    auto make_terminal (T && t)
    {
        static_assert(
            !detail::is_expr<T>::value,
            "make_terminal() is only defined for non expressions."
        );
        using result_type = detail::operand_type_t<ExprTemplate, T>;
        using tuple_type = decltype(std::declval<result_type>().elements);
        return result_type{tuple_type{static_cast<T &&>(t)}};
    }

    template <typename T>
    auto make_terminal (T && t)
    { return make_terminal<expression>(static_cast<T &&>(t)); }

    template <template <expr_kind, class> class ExprTemplate, typename T>
    decltype(auto) as_expr (T && t)
    {
        if constexpr (detail::is_expr<T>::value) {
            return static_cast<T &&>(t);
        } else {
            return make_terminal<ExprTemplate>(static_cast<T &&>(t));
        }
    }

    template <typename T>
    decltype(auto) as_expr (T && t)
    { return as_expr<expression>(static_cast<T &&>(t)); }

    template <typename Expr>
    struct expression_function
    {
        template <typename ...U>
        decltype(auto) operator() (U &&... u)
        { return ::boost::yap::evaluate(expr, static_cast<U &&>(u)...); }

        Expr expr;
    };

    template <typename Expr>
    auto make_expression_function (Expr && expr)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "make_expression_function() is only defined for expressions."
        );
        return expression_function<Expr>{std::move(expr)};
    }

}

#include <boost/yap/detail/default_eval.hpp>

namespace boost::yap {

    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr const & expr, T && ...t)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "evaluate() is only defined for expressions."
        );
        return detail::default_eval_expr(expr, static_cast<T &&>(t)...);
    }

    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr const & expr, T && ...t)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "evaluate_as() is only defined for expressions."
        );
        return eval_expression_as(expr, hana::basic_type<R>{}, static_cast<T &&>(t)...);
    }

    template <typename Expr, typename Transform>
    decltype(auto) transform (Expr && expr, Transform && transform)
    {
        if constexpr (detail::is_expr<Expr>::value) {
            constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
            return detail::default_transform_expression<Expr, Transform, detail::arity_of<kind>()>{}(
                static_cast<Expr &&>(expr),
                static_cast<Transform &&>(transform)
            );
        } else {
            return static_cast<Expr &&>(expr);
        }
    }

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T &&... args)
        { return static_cast<R>(detail::default_eval_expr(expr, static_cast<T &&>(args)...)); }

    }

}

#endif
