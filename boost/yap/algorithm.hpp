#ifndef BOOST_YAP_ALGORITHM_HPP_INCLUDED
#define BOOST_YAP_ALGORITHM_HPP_INCLUDED

#include <boost/yap/algorithm_fwd.hpp>
#include <boost/yap/user_macros.hpp>
#include <boost/yap/detail/algorithm.hpp>

#include <boost/hana/size.hpp>
#include <boost/hana/comparing.hpp>


namespace boost { namespace yap {

#ifndef BOOST_YAP_DOXYGEN

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T && ... args);

        struct eval_expression_as_fn
        {
            template <typename R, typename E, typename ...T>
            constexpr decltype(auto) operator() (E const & expr, hana::basic_type<R> rtype, T && ... args) const
            { return eval_expression_as(expr, rtype, static_cast<T &&>(args)...); }
        };

    }

    using adl_detail::eval_expression_as_fn;

    inline namespace {

        constexpr auto & eval_expression_as = detail::static_const<eval_expression_as_fn>::value;

    }

#endif

#ifdef BOOST_NO_CONSTEXPR_IF

    namespace detail {

        template <typename Expr, bool MutableRvalueRef>
        struct deref_impl
        {
            decltype(auto) operator() (Expr && expr)
            { return std::move(*expr.elements[hana::llong_c<0>]); }
        };

        template <typename Expr>
        struct deref_impl<Expr, false>
        {
            decltype(auto) operator() (Expr && expr)
            { return *expr.elements[hana::llong_c<0>]; }
        };

    }

#endif

    /** "Dereferences" a reference-expression, forwarding its referent to the
        caller. */
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

#ifdef BOOST_NO_CONSTEXPR_IF
        return detail::deref_impl<
            Expr,
            std::is_rvalue_reference<Expr>{} &&
            !std::is_const<std::remove_reference_t<Expr>>{}
        >{}(static_cast<Expr &&>(expr));
#else
        using namespace hana::literals;
        if constexpr (
            std::is_rvalue_reference<Expr>{} &&
            !std::is_const<std::remove_reference_t<Expr>>{}
        ) {
            return std::move(*expr.elements[0_c]);
        } else {
            return *expr.elements[0_c];
        }
#endif
    }

    namespace detail {

#ifdef BOOST_NO_CONSTEXPR_IF

        template <bool ValueOfTerminalsOnly, typename T>
        decltype(auto) value_impl (T && x);

        template <typename T, bool IsExprRef, bool ValueOfTerminalsOnly, bool TakeValue, bool IsLvalueRef>
        struct value_expr_impl;

        template <typename T, bool ValueOfTerminalsOnly, bool TakeValue, bool IsLvalueRef>
        struct value_expr_impl<T, true, ValueOfTerminalsOnly, TakeValue, IsLvalueRef>
        {
            decltype(auto) operator() (T && x)
            { return value_impl<ValueOfTerminalsOnly>(::boost::yap::deref(static_cast<T &&>(x))); }
        };

        template <typename T, bool ValueOfTerminalsOnly>
        struct value_expr_impl<T, false, ValueOfTerminalsOnly, true, true>
        {
            decltype(auto) operator() (T && x)
            { return x.elements[hana::llong_c<0>]; }
        };

        template <typename T, bool ValueOfTerminalsOnly>
        struct value_expr_impl<T, false, ValueOfTerminalsOnly, true, false>
        {
            decltype(auto) operator() (T && x)
            { return std::move(x.elements[hana::llong_c<0>]); }
        };

        template <typename T, bool ValueOfTerminalsOnly, bool IsLvalueRef>
        struct value_expr_impl<T, false, ValueOfTerminalsOnly, false, IsLvalueRef>
        {
            decltype(auto) operator() (T && x)
            { return static_cast<T &&>(x); }
        };

        template <typename T, bool IsExpr, bool ValueOfTerminalsOnly>
        struct value_impl_t
        {
            decltype(auto) operator() (T && x)
            {
                constexpr expr_kind kind = detail::remove_cv_ref_t<T>::kind;
                constexpr detail::expr_arity arity = detail::arity_of<kind>();
                return value_expr_impl<
                    T,
                    kind == expr_kind::expr_ref,
                    ValueOfTerminalsOnly,
                    (ValueOfTerminalsOnly && kind == expr_kind::terminal) ||
                    (!ValueOfTerminalsOnly && arity == detail::expr_arity::one),
                    std::is_lvalue_reference<T>{}
                >{}(static_cast<T &&>(x));
            }
        };

        template <typename T, bool ValueOfTerminalsOnly>
        struct value_impl_t<T, false, ValueOfTerminalsOnly>
        {
            decltype(auto) operator() (T && x)
            { return static_cast<T &&>(x); }
        };

        template <bool ValueOfTerminalsOnly, typename T>
        decltype(auto) value_impl (T && x)
        {
            return detail::value_impl_t<T, detail::is_expr<T>::value, ValueOfTerminalsOnly>{}(
                static_cast<T &&>(x)
            );
        }

#else

        template <bool ValueOfTerminalsOnly, typename T>
        decltype(auto) value_impl (T && x)
        {
            if constexpr (is_expr<T>::value) {
                using namespace hana::literals;
                constexpr expr_kind kind = remove_cv_ref_t<T>::kind;
                constexpr expr_arity arity = arity_of<kind>();
                if constexpr (kind == expr_kind::expr_ref) {
                    return value_impl<ValueOfTerminalsOnly>(
                        ::boost::yap::deref(static_cast<T &&>(x))
                    );
                } else if constexpr ((ValueOfTerminalsOnly && kind == expr_kind::terminal) ||
                                     (!ValueOfTerminalsOnly && arity == expr_arity::one)) {
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

#endif

    }

    /** Forwards the sole element of \a x to the caller, possibly calling
        <code>deref()</code> first if \a x is a reference expression, or
        forwards \a x to the caller unchanged.

        More formally:

        - If \a x is not an expression, \a x is forwarded to the caller.

        - Otherwise, if \a x is a reference expression, the result is
        <code>value(deref(x))</code>.

        - Otherwise, if \a x is an expression with only one value (a unary
        expression or a terminal expression), the result is the forwarded
        first element of \a x.

        - Otherwise, \a x is forwarded to the caller. */
    template <typename T>
    decltype(auto) value (T && x)
    { return detail::value_impl<false>(static_cast<T &&>(x)); }

#ifdef BOOST_NO_CONSTEXPR_IF

    template <long long I, typename Expr>
    decltype(auto) get (Expr && expr, hana::llong<I> i);

    namespace detail {

        template <long long I, typename Expr, bool IsExpr, bool IsLvalueRef>
        struct get_impl;

        template <long long I, typename Expr, bool IsLvalueRef>
        struct get_impl<I, Expr, true, IsLvalueRef>
        {
            decltype(auto) operator() (Expr && expr, hana::llong<I> i)
            { return ::boost::yap::get(::boost::yap::deref(static_cast<Expr &&>(expr)), i); }
        };

        template <long long I, typename Expr>
        struct get_impl<I, Expr, false, true>
        {
            decltype(auto) operator() (Expr && expr, hana::llong<I> i)
            { return expr.elements[i]; }
        };

        template <long long I, typename Expr>
        struct get_impl<I, Expr, false, false>
        {
            decltype(auto) operator() (Expr && expr, hana::llong<I> i)
            { return std::move(expr.elements[i]); }
        };

    }

#endif

    /** Forwards the <i>i</i>-th element of \a expr to the caller.  If \a expr
        is a reference expression, the result is <code>get(deref(expr),
        i)</code>.

        \note <code>get()</code> is only valid if \a Expr is an expression.
    */
    template <long long I, typename Expr>
    decltype(auto) get (Expr && expr, hana::llong<I> i)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "get() is only defined for expressions."
        );

        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;

        static_assert(
            kind == expr_kind::expr_ref ||
            (0 <= I && I < decltype(hana::size(expr.elements))::value),
            "In get(expr, I), I must be nonnegative, and less "
            "than hana::size(expr.elements)."
        );

#ifdef BOOST_NO_CONSTEXPR_IF
        return detail::get_impl<
            I,
            Expr,
            kind == expr_kind::expr_ref,
            std::is_lvalue_reference<Expr>{}
        >{}(static_cast<Expr &&>(expr), i);
#else
        using namespace hana::literals;
        if constexpr (kind == expr_kind::expr_ref) {
            return ::boost::yap::get(::boost::yap::deref(static_cast<Expr &&>(expr)), i);
        } else {
            if constexpr (std::is_lvalue_reference<Expr>{}) {
                return expr.elements[i];
            } else {
                return std::move(expr.elements[i]);
            }
        }
#endif
    }

    /** Returns <code>get(expr, boost::hana::llong_c<I>)</code>. */
    template <long long I, typename Expr>
    decltype(auto) get_c (Expr && expr)
    { return ::boost::yap::get(static_cast<Expr &&>(expr), hana::llong_c<I>); }

    /** Returns the left operand in a binary operator expression.

        Equivalent to <code>get(expr, 0_c)</code>.

        \note <code>left()</code> is only valid if \a Expr is a binary
        operator expression.
    */
    template <typename Expr>
    decltype(auto) left (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 0_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        static_assert(
            kind == expr_kind::expr_ref ||
            detail::arity_of<kind>() == detail::expr_arity::two,
            "left() is only defined for binary expressions."
        );
    }

    /** Returns the right operand in a binary operator expression.

        Equivalent to <code>get(expr, 1_c)</code>.

        \note <code>right()</code> is only valid if \a Expr is a binary
        operator expression.
    */
    template <typename Expr>
    decltype(auto) right (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 1_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        static_assert(
            kind == expr_kind::expr_ref ||
            detail::arity_of<kind>() == detail::expr_arity::two,
            "right() is only defined for binary expressions."
        );
    }

    /** Returns the condition expression in an if_else expression.

        Equivalent to <code>get(expr, 0_c)</code>.

        \note <code>cond()</code> is only valid if \a Expr is an
        <code>expr_kind::if_else</code> expression.
    */
    template <typename Expr>
    decltype(auto) cond (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 0_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        static_assert(
            kind == expr_kind::expr_ref || kind == expr_kind::if_else,
            "cond() is only defined for if_else expressions."
        );
    }

    /** Returns the then-expression in an if_else expression.

        Equivalent to <code>get(expr, 1_c)</code>.

        \note <code>then()</code> is only valid if \a Expr is an
        <code>expr_kind::if_else</code> expression.
    */
    template <typename Expr>
    decltype(auto) then (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 1_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        static_assert(
            kind == expr_kind::expr_ref || kind == expr_kind::if_else,
            "then() is only defined for if_else expressions."
        );
    }

    /** Returns the else-expression in an if_else expression.

        Equivalent to <code>get(expr, 2_c)</code>.

        \note <code>else_()</code> is only valid if \a Expr is an
        <code>expr_kind::if_else</code> expression.
    */
    template <typename Expr>
    decltype(auto) else_ (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 2_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        static_assert(
            kind == expr_kind::expr_ref || kind == expr_kind::if_else,
            "else_() is only defined for if_else expressions."
        );
    }

    /** Returns the callable in a call expression.

        Equivalent to <code>get(expr, 0)</code>.

        \note <code>callable()</code> is only valid if \a Expr is an
        <code>expr_kind::call</code> expression.
    */
    template <typename Expr>
    decltype(auto) callable (Expr && expr)
    {
        return ::boost::yap::get(static_cast<Expr &&>(expr), hana::llong_c<0>);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        static_assert(
            kind == expr_kind::expr_ref ||
            detail::arity_of<kind>() == detail::expr_arity::n,
            "callable() is only defined for call expressions."
        );
    }

    /** Returns the <i>i-th</i> argument expression in a call expression.

        Equivalent to <code>get(expr, i + 1)</code>.

        \note <code>argument()</code> is only valid if \a Expr is an
        <code>expr_kind::call</code> expression.
    */
    template <long long I, typename Expr>
    decltype(auto) argument (Expr && expr, hana::llong<I> i)
    {
        return ::boost::yap::get(static_cast<Expr &&>(expr), hana::llong_c<I + 1>);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        static_assert(
            kind == expr_kind::expr_ref ||
            detail::arity_of<kind>() == detail::expr_arity::n,
            "argument() is only defined for call expressions."
        );
        static_assert(
            kind == expr_kind::expr_ref ||
            (0 <= I && I < decltype(hana::size(expr.elements))::value - 1),
            "In argument(expr, I), I must be nonnegative, and less "
            "than hana::size(expr.elements)."
        );
    }

    /** Makes a new expression instantiated from the expression template \a
        ExprTemplate, of kind \a Kind, with the given values as its elements.

        For each parameter P:

        - If P is an expression, P is moved into the result if P is an rvalue
        and captured by reference into the result otherwise.

        - Otherwise, P is wrapped in a terminal expression.

        \note <code>make_expression()</code> is only valid if the number of
        parameters passed is appropriate for \a Kind.
    */
    template <template <expr_kind, class> class ExprTemplate, expr_kind Kind, typename ...T>
    auto make_expression (T && ... t)
    {
        constexpr detail::expr_arity arity = detail::arity_of<Kind>();
        static_assert(
            (arity == detail::expr_arity::one && sizeof...(T) == 1) ||
            (arity == detail::expr_arity::two && sizeof...(T) == 2) ||
            (arity == detail::expr_arity::three && sizeof...(T) == 3) ||
            arity == detail::expr_arity::n,
            "The number of parameters passed to make_expression() must match the arity "
            "implied by the expr_kind template parameter."
        );
        using tuple_type = hana::tuple<detail::operand_type_t<ExprTemplate, T>...>;
        return ExprTemplate<Kind, tuple_type>{
            tuple_type{
                detail::make_operand<detail::operand_type_t<ExprTemplate, T>>{}(static_cast<T &&>(t))...
            }
        };
    }

    /** Returns <code>make_expression<boost::yap::expression, Kind>(...)</code>. */
    template <expr_kind Kind, typename ...T>
    auto make_expression (T && ... t)
    { return make_expression<expression, Kind>(static_cast<T &&>(t)...); }

    /** Makes a new terminal expression instantiated from the expression
        template \a ExprTemplate, with the given value as its sole element.

        \note <code>make_terminal()</code> is only valid if \a T is \b not an
        expression.
    */
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

    /** Returns <code>make_terminal<boost::yap::expression>(t)</code>. */
    template <typename T>
    auto make_terminal (T && t)
    { return make_terminal<expression>(static_cast<T &&>(t)); }

#ifdef BOOST_NO_CONSTEXPR_IF

    namespace detail {

        template <template <expr_kind, class> class ExprTemplate, typename T, bool IsExpr>
        struct as_expr_impl
        {
            decltype(auto) operator() (T && t)
            { return static_cast<T &&>(t); }
        };

        template <template <expr_kind, class> class ExprTemplate, typename T>
        struct as_expr_impl<ExprTemplate, T, false>
        {
            decltype(auto) operator() (T && t)
            { return make_terminal<ExprTemplate>(static_cast<T &&>(t)); }
        };

    }

#endif

    /** Returns an expression formed from \a t as follows:

        - If \a t is an expression, \a t is forwarded to the caller.

        - Otherwise, \a t is wrapped in a terminal expression.
    */
    template <template <expr_kind, class> class ExprTemplate, typename T>
    decltype(auto) as_expr (T && t)
    {
#ifdef BOOST_NO_CONSTEXPR_IF
        return detail::as_expr_impl<ExprTemplate, T, detail::is_expr<T>::value>{}(
            static_cast<T &&>(t)
        );
#else
        if constexpr (detail::is_expr<T>::value) {
            return static_cast<T &&>(t);
        } else {
            return make_terminal<ExprTemplate>(static_cast<T &&>(t));
        }
#endif
    }

    /** Returns <code>as_expr<boost::yap::expression>(t)</code>. */
    template <typename T>
    decltype(auto) as_expr (T && t)
    { return as_expr<expression>(static_cast<T &&>(t)); }

    /** A callable type that evaluates its contained expression when called.

        \see <code>make_expression_function()</code>
    */
    template <typename Expr>
    struct expression_function
    {
        template <typename ...U>
        decltype(auto) operator() (U && ... u)
        { return ::boost::yap::evaluate(expr, static_cast<U &&>(u)...); }

        Expr expr;
    };

    /** Returns a callable object that \a expr has been moved into.  This is
        useful for using expressions as function objects.

        \note <code>make_expression_function()</code> is only valid if \a
        Expr is an expression.
    */
    template <typename Expr>
    auto make_expression_function (Expr && expr)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "make_expression_function() is only defined for expressions."
        );
        return expression_function<Expr>{std::move(expr)};
    }

} }

#include <boost/yap/detail/default_eval.hpp>

namespace boost { namespace yap {

    /** Evaluates \a expr, substituting the subsequent parameters (if any)
        into \a expr's placeholder terminals.

        All customization points for the evaluation of expressions \b except
        <code>eval_expression_as()</code> are used to evaluate the \a expr.
        If you've overridden any, that will be reflected in the result.

        \note <code>evaluate()</code> is only valid if \a Expr is an
        expression, and <code>max_p <= sizeof...(T)</code>, where
        <code>max_p</code> is the maximum placeholder index in \a expr.
    */
    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr && expr, T && ... t)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "evaluate() is only defined for expressions."
        );
        return detail::default_eval_expr(static_cast<Expr &&>(expr), static_cast<T &&>(t)...);
    }

    /** Evaluates \a expr, substituting the subsequent parameters (if any)
        into \a expr's placeholder terminals.  Evaluation is performed via the
        <code>eval_expression_as()</code> customization point.

        Prefer this function to <code>evaluate()</code> when you want
        evaluation to differ based on the result type.

        All customization points for the evaluation of expressions are used to
        evaluate the \a expr.  If you've overridden any, that will be
        reflected in the result.

        \note <code>evaluate()</code> is only valid if \a Expr is an
        expression, and <code>max_p <= sizeof...(T)</code>, where
        <code>max_p</code> is the maximum placeholder index in \a expr.
    */
    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr && expr, T && ... t)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "evaluate_as() is only defined for expressions."
        );
        return eval_expression_as(static_cast<Expr &&>(expr), hana::basic_type<R>{}, static_cast<T &&>(t)...);
    }

#ifdef BOOST_NO_CONSTEXPR_IF

    namespace detail {

        template <typename Expr, typename Transform, bool IsExpr>
        struct transform_impl
        {
            decltype(auto) operator() (Expr && expr, Transform && transform)
            {
                constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
                return detail::default_transform_expression_tag<Expr, Transform, detail::arity_of<kind>()>{}(
                    static_cast<Expr &&>(expr),
                    static_cast<Transform &&>(transform)
                );
            }
        };

        template <typename Expr, typename Transform>
        struct transform_impl<Expr, Transform, false>
        {
            decltype(auto) operator() (Expr && expr, Transform && transform)
            { return static_cast<Expr &&>(expr); }
        };

    }

#endif

    /** Returns the result of transforming (all or part of) \a expr using
        whatever overloads of <code>Transform::operator()</code> that match \a
        expr.

        \note Transformations can do anything: they may have side effects;
        they may mutate values; they may mutate types; and they may do any
        combination of these.
    */
    template <typename Expr, typename Transform>
    decltype(auto) transform (Expr && expr, Transform && transform)
    {
#ifdef BOOST_NO_CONSTEXPR_IF
        return detail::transform_impl<Expr, Transform, detail::is_expr<Expr>::value>{}(
            static_cast<Expr &&>(expr),
            static_cast<Transform &&>(transform)
        );
#else
        if constexpr (detail::is_expr<Expr>::value) {
            constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
            return detail::default_transform_expression_tag<Expr, Transform, detail::arity_of<kind>()>{}(
                static_cast<Expr &&>(expr),
                static_cast<Transform &&>(transform)
            );
        } else {
            return static_cast<Expr &&>(expr);
        }
#endif
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

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T && ... args)
        { return static_cast<R>(detail::default_eval_expr(expr, static_cast<T &&>(args)...)); }

    }

} }

#endif
