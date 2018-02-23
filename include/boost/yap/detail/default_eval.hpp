// Copyright (C) 2016-2018 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_YAP_DETAIL_DEFAULT_EVAL_HPP_INCLUDED
#define BOOST_YAP_DETAIL_DEFAULT_EVAL_HPP_INCLUDED

#include <boost/yap/algorithm_fwd.hpp>

#include <boost/hana/transform.hpp>

#include <cassert>


namespace boost { namespace yap { namespace detail {

    template<typename F, typename... T>
    constexpr auto eval_call(F && f, T &&... t)
    {
        return static_cast<F &&>(f)(static_cast<T &&>(t)...);
    }

    template<typename I, typename T>
    decltype(auto) eval_placeholder(I, T && arg)
    {
        static_assert(
            I::value == 1,
            "The current placeholder's index is out of bounds.  Did you "
            "perhaps call "
            "evaluate() with too few arguments?");
        return static_cast<T &&>(arg);
    }

#ifdef BOOST_NO_CONSTEXPR_IF

    template<typename T, typename... Ts>
    decltype(auto) eval_placeholder(hana::llong<1>, T && arg, Ts &&... args)
    {
        return static_cast<T &&>(arg);
    }

    template<typename I, typename T, typename... Ts>
    decltype(auto) eval_placeholder(I, T && arg, Ts &&... args)
    {
        return eval_placeholder(
            hana::llong<I::value - 1>{}, static_cast<Ts &&>(args)...);
    }

#else

    template<typename I, typename T, typename... Ts>
    decltype(auto) eval_placeholder(I, T && arg, Ts &&... args)
    {
        if constexpr (I::value == 1) {
            return static_cast<T &&>(arg);
        } else {
            return eval_placeholder(
                hana::llong<I::value - 1>{}, static_cast<Ts &&>(args)...);
        }
    }

#endif

    template<long long I, typename... T>
    decltype(auto) eval_terminal(placeholder<I>, T &&... args)
    {
        return eval_placeholder(hana::llong_c<I>, static_cast<T &&>(args)...);
    }

    template<typename T, typename... Ts>
    decltype(auto) eval_terminal(T && value, Ts &&... args)
    {
        return static_cast<T &&>(value);
    }

#ifdef BOOST_NO_CONSTEXPR_IF

    template<typename Expr, typename... T>
    decltype(auto) default_eval_expr(Expr && expr, T &&... args);

    template<expr_kind Kind>
    struct default_eval_expr_impl;

    template<>
    struct default_eval_expr_impl<expr_kind::expr_ref>
    {
        template<typename Expr, typename... T>
        decltype(auto) operator()(Expr && expr, T &&... args)
        {
            return default_eval_expr(
                ::boost::yap::deref(static_cast<Expr &&>(expr)),
                static_cast<T &&>(args)...);
        }
    };

    template<>
    struct default_eval_expr_impl<expr_kind::terminal>
    {
        template<typename Expr, typename... T>
        decltype(auto) operator()(Expr && expr, T &&... args)
        {
            return eval_terminal(
                ::boost::yap::value(static_cast<Expr &&>(expr)),
                static_cast<T &&>(args)...);
        }
    };

#define BOOST_YAP_UNARY_OPERATOR_CASE(op, op_name)                             \
    template<>                                                                 \
    struct default_eval_expr_impl<expr_kind::op_name>                          \
    {                                                                          \
        template<typename Expr, typename... T>                                 \
        decltype(auto) operator()(Expr && expr, T &&... args)                  \
        {                                                                      \
            using namespace hana::literals;                                    \
            return op default_eval_expr(                                       \
                static_cast<Expr &&>(expr).elements[0_c],                      \
                static_cast<T &&>(args)...);                                   \
        }                                                                      \
    };

    BOOST_YAP_UNARY_OPERATOR_CASE(+, unary_plus)
    BOOST_YAP_UNARY_OPERATOR_CASE(-, negate)
    BOOST_YAP_UNARY_OPERATOR_CASE(*, dereference)
    BOOST_YAP_UNARY_OPERATOR_CASE(~, complement)
    BOOST_YAP_UNARY_OPERATOR_CASE(&, address_of)
    BOOST_YAP_UNARY_OPERATOR_CASE(!, logical_not)
    BOOST_YAP_UNARY_OPERATOR_CASE(++, pre_inc)
    BOOST_YAP_UNARY_OPERATOR_CASE(--, pre_dec)

    template<>
    struct default_eval_expr_impl<expr_kind::post_inc>
    {
        template<typename Expr, typename... T>
        decltype(auto) operator()(Expr && expr, T &&... args)
        {
            using namespace hana::literals;
            return default_eval_expr(
                static_cast<Expr &&>(expr).elements[0_c],
                static_cast<T &&>(args)...)++;
        }
    };

    template<>
    struct default_eval_expr_impl<expr_kind::post_dec>
    {
        template<typename Expr, typename... T>
        decltype(auto) operator()(Expr && expr, T &&... args)
        {
            using namespace hana::literals;
            return default_eval_expr(
                static_cast<Expr &&>(expr).elements[0_c],
                static_cast<T &&>(args)...)--;
        }
    };

#undef BOOST_YAP_UNARY_OPERATOR_CASE

#define BOOST_YAP_BINARY_OPERATOR_CASE(op, op_name)                            \
    template<>                                                                 \
    struct default_eval_expr_impl<expr_kind::op_name>                          \
    {                                                                          \
        template<typename Expr, typename... T>                                 \
        decltype(auto) operator()(Expr && expr, T &&... args)                  \
        {                                                                      \
            using namespace hana::literals;                                    \
            return default_eval_expr(                                          \
                static_cast<Expr &&>(expr).elements[0_c],                      \
                static_cast<T &&>(args)...) op                                 \
            default_eval_expr(                                                 \
                static_cast<Expr &&>(expr).elements[1_c],                      \
                static_cast<T &&>(args)...);                                   \
        }                                                                      \
    };

    BOOST_YAP_BINARY_OPERATOR_CASE(<<, shift_left)
    BOOST_YAP_BINARY_OPERATOR_CASE(>>, shift_right)
    BOOST_YAP_BINARY_OPERATOR_CASE(*, multiplies)
    BOOST_YAP_BINARY_OPERATOR_CASE(/, divides)
    BOOST_YAP_BINARY_OPERATOR_CASE(%, modulus)
    BOOST_YAP_BINARY_OPERATOR_CASE(+, plus)
    BOOST_YAP_BINARY_OPERATOR_CASE(-, minus)
    BOOST_YAP_BINARY_OPERATOR_CASE(<, less)
    BOOST_YAP_BINARY_OPERATOR_CASE(>, greater)
    BOOST_YAP_BINARY_OPERATOR_CASE(<=, less_equal)
    BOOST_YAP_BINARY_OPERATOR_CASE(>=, greater_equal)
    BOOST_YAP_BINARY_OPERATOR_CASE(==, equal_to)
    BOOST_YAP_BINARY_OPERATOR_CASE(!=, not_equal_to)
    BOOST_YAP_BINARY_OPERATOR_CASE(||, logical_or)
    BOOST_YAP_BINARY_OPERATOR_CASE(&&, logical_and)
    BOOST_YAP_BINARY_OPERATOR_CASE(&, bitwise_and)
    BOOST_YAP_BINARY_OPERATOR_CASE(|, bitwise_or)
    BOOST_YAP_BINARY_OPERATOR_CASE (^, bitwise_xor)

    template<>
    struct default_eval_expr_impl<expr_kind::comma>
    {
        template<typename Expr, typename... T>
        decltype(auto) operator()(Expr && expr, T &&... args)
        {
            using namespace hana::literals;
            return default_eval_expr(
                       static_cast<Expr &&>(expr).elements[0_c],
                       static_cast<T &&>(args)...),
                   default_eval_expr(
                       static_cast<Expr &&>(expr).elements[1_c],
                       static_cast<T &&>(args)...);
        }
    };

    BOOST_YAP_BINARY_OPERATOR_CASE(->*, mem_ptr)
    BOOST_YAP_BINARY_OPERATOR_CASE(=, assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(<<=, shift_left_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(>>=, shift_right_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(*=, multiplies_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(/=, divides_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(%=, modulus_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(+=, plus_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(-=, minus_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(&=, bitwise_and_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(|=, bitwise_or_assign)
    BOOST_YAP_BINARY_OPERATOR_CASE(^=, bitwise_xor_assign)

    template<>
    struct default_eval_expr_impl<expr_kind::subscript>
    {
        template<typename Expr, typename... T>
        decltype(auto) operator()(Expr && expr, T &&... args)
        {
            using namespace hana::literals;
            return default_eval_expr(
                static_cast<Expr &&>(expr).elements[0_c],
                static_cast<T &&>(args)...)[default_eval_expr(
                static_cast<Expr &&>(expr).elements[1_c],
                static_cast<T &&>(args)...)];
        }
    };

#undef BOOST_YAP_BINARY_OPERATOR_CASE

    template<>
    struct default_eval_expr_impl<expr_kind::if_else>
    {
        template<typename Expr, typename... T>
        decltype(auto) operator()(Expr && expr, T &&... args)
        {
            using namespace hana::literals;
            if (default_eval_expr(
                    static_cast<Expr &&>(expr).elements[0_c],
                    static_cast<T &&>(args)...)) {
                return default_eval_expr(
                    static_cast<Expr &&>(expr).elements[1_c],
                    static_cast<T &&>(args)...);
            } else {
                return default_eval_expr(
                    static_cast<Expr &&>(expr).elements[2_c],
                    static_cast<T &&>(args)...);
            }
        }
    };

    template<>
    struct default_eval_expr_impl<expr_kind::call>
    {
        template<typename Expr, typename... T>
        decltype(auto) operator()(Expr && expr, T &&... args)
        {
            decltype(auto) expand_args = [&](auto && element) {
                return default_eval_expr(
                    static_cast<decltype(element) &&>(element),
                    static_cast<T &&>(args)...);
            };

            return hana::unpack(
                static_cast<Expr &&>(expr).elements,
                [expand_args](auto &&... elements) {
                    return eval_call(expand_args(
                        static_cast<decltype(elements) &&>(elements))...);
                });
        }
    };

    template<typename Expr, typename... T>
    decltype(auto) default_eval_expr(Expr && expr, T &&... args)
    {
        constexpr expr_kind kind = remove_cv_ref_t<Expr>::kind;

        return default_eval_expr_impl<kind>{}(
            static_cast<Expr &&>(expr), static_cast<T &&>(args)...);
    }

#else // BOOST_NO_CONSTEXPR_IF

    template<typename Expr, typename... T>
    decltype(auto) default_eval_expr(Expr && expr, T &&... args)
    {
        constexpr expr_kind kind = remove_cv_ref_t<Expr>::kind;

        using namespace hana::literals;

        if constexpr (kind == expr_kind::expr_ref) {
            return default_eval_expr(
                ::boost::yap::deref(static_cast<Expr &&>(expr)),
                static_cast<T &&>(args)...);
        } else if constexpr (kind == expr_kind::terminal) {
            return eval_terminal(
                ::boost::yap::value(static_cast<Expr &&>(expr)),
                static_cast<T &&>(args)...);
        }

#define BOOST_YAP_UNARY_OPERATOR_CASE(op, op_name)                             \
    else if constexpr (kind == expr_kind::op_name)                             \
    {                                                                          \
        return op default_eval_expr(                                           \
            static_cast<Expr &&>(expr).elements[0_c],                          \
            static_cast<T &&>(args)...);                                       \
    }

        BOOST_YAP_UNARY_OPERATOR_CASE(+, unary_plus)
        BOOST_YAP_UNARY_OPERATOR_CASE(-, negate)
        BOOST_YAP_UNARY_OPERATOR_CASE(*, dereference)
        BOOST_YAP_UNARY_OPERATOR_CASE(~, complement)
        BOOST_YAP_UNARY_OPERATOR_CASE(&, address_of)
        BOOST_YAP_UNARY_OPERATOR_CASE(!, logical_not)
        BOOST_YAP_UNARY_OPERATOR_CASE(++, pre_inc)
        BOOST_YAP_UNARY_OPERATOR_CASE(--, pre_dec)

        else if constexpr (kind == expr_kind::post_inc)
        {
            return default_eval_expr(
                static_cast<Expr &&>(expr).elements[0_c],
                static_cast<T &&>(args)...)++;
        }
        else if constexpr (kind == expr_kind::post_dec)
        {
            return default_eval_expr(
                static_cast<Expr &&>(expr).elements[0_c],
                static_cast<T &&>(args)...)--;
        }

#undef BOOST_YAP_UNARY_OPERATOR_CASE

#define BOOST_YAP_BINARY_OPERATOR_CASE(op, op_name)                            \
    else if constexpr (kind == expr_kind::op_name)                             \
    {                                                                          \
        return default_eval_expr(                                              \
            static_cast<Expr &&>(expr).elements[0_c],                          \
            static_cast<T &&>(args)...) op                                     \
        default_eval_expr(                                                     \
            static_cast<Expr &&>(expr).elements[1_c],                          \
            static_cast<T &&>(args)...);                                       \
    }

        BOOST_YAP_BINARY_OPERATOR_CASE(<<, shift_left)
        BOOST_YAP_BINARY_OPERATOR_CASE(>>, shift_right)
        BOOST_YAP_BINARY_OPERATOR_CASE(*, multiplies)
        BOOST_YAP_BINARY_OPERATOR_CASE(/, divides)
        BOOST_YAP_BINARY_OPERATOR_CASE(%, modulus)
        BOOST_YAP_BINARY_OPERATOR_CASE(+, plus)
        BOOST_YAP_BINARY_OPERATOR_CASE(-, minus)
        BOOST_YAP_BINARY_OPERATOR_CASE(<, less)
        BOOST_YAP_BINARY_OPERATOR_CASE(>, greater)
        BOOST_YAP_BINARY_OPERATOR_CASE(<=, less_equal)
        BOOST_YAP_BINARY_OPERATOR_CASE(>=, greater_equal)
        BOOST_YAP_BINARY_OPERATOR_CASE(==, equal_to)
        BOOST_YAP_BINARY_OPERATOR_CASE(!=, not_equal_to)
        BOOST_YAP_BINARY_OPERATOR_CASE(||, logical_or)
        BOOST_YAP_BINARY_OPERATOR_CASE(&&, logical_and)
        BOOST_YAP_BINARY_OPERATOR_CASE(&, bitwise_and)
        BOOST_YAP_BINARY_OPERATOR_CASE(|, bitwise_or)
        BOOST_YAP_BINARY_OPERATOR_CASE (^, bitwise_xor)

        else if constexpr (kind == expr_kind::comma)
        {
            return default_eval_expr(
                       static_cast<Expr &&>(expr).elements[0_c],
                       static_cast<T &&>(args)...),
                   default_eval_expr(
                       static_cast<Expr &&>(expr).elements[1_c],
                       static_cast<T &&>(args)...);
        }

        BOOST_YAP_BINARY_OPERATOR_CASE(->*, mem_ptr)
        BOOST_YAP_BINARY_OPERATOR_CASE(=, assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(<<=, shift_left_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(>>=, shift_right_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(*=, multiplies_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(/=, divides_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(%=, modulus_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(+=, plus_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(-=, minus_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(&=, bitwise_and_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(|=, bitwise_or_assign)
        BOOST_YAP_BINARY_OPERATOR_CASE(^=, bitwise_xor_assign)

        else if constexpr (kind == expr_kind::if_else)
        {
            if (default_eval_expr(
                    static_cast<Expr &&>(expr).elements[0_c],
                    static_cast<T &&>(args)...)) {
                return default_eval_expr(
                    static_cast<Expr &&>(expr).elements[1_c],
                    static_cast<T &&>(args)...);
            } else {
                return default_eval_expr(
                    static_cast<Expr &&>(expr).elements[2_c],
                    static_cast<T &&>(args)...);
            }
        }

#undef BOOST_YAP_BINARY_OPERATOR_CASE

        else if constexpr (kind == expr_kind::call)
        {
            decltype(auto) expand_args = [&](auto && element) {
                return default_eval_expr(
                    static_cast<decltype(element) &&>(element),
                    static_cast<T &&>(args)...);
            };

            return hana::unpack(
                static_cast<Expr &&>(expr).elements,
                [expand_args](auto &&... elements) {
                    return eval_call(expand_args(
                        static_cast<decltype(elements) &&>(elements))...);
                });
        }
        else
        {
            assert(false && "Unhandled expr_kind in default_evaluate!");
            return;
        }
    }

#endif // BOOST_NO_CONSTEXPR_IF


    template<bool Strict, typename Expr, typename Transform, bool IsExprRef>
    struct transform_impl;

    template<
        bool Strict,
        typename Expr,
        typename Transform,
        expr_arity Arity,
        typename = void_t<>>
    struct transform_expression_tag;


    // Forward terminals/recurively transform noterminasl; attempted last.

    template<bool IsLvalueRef, bool IsTerminal, bool Strict>
    struct default_transform
    {
        template<typename Expr, typename Transform>
        decltype(auto) operator()(Expr && expr, Transform & transform) const
        {
            return static_cast<Expr &&>(expr);
        }
    };

    template<bool IsLvalueRef, bool IsTerminal>
    struct default_transform<IsLvalueRef, IsTerminal, true>
    {
        struct incomplete;

        // If you're getting an error because this function is uncallable,
        // that's by design.  You called yap::transform_strict(expr, xfrom)
        // and one or more subexpression of 'expr' are not callable with any
        // overload in 'xform'.
        template<typename Expr, typename Transform>
        incomplete operator()(Expr && expr, Transform & transform) const;
    };

    template<
        template<expr_kind, class> class ExprTemplate,
        expr_kind Kind,
        typename OldTuple,
        typename NewTuple>
    auto make_expr_from_tuple(
        ExprTemplate<Kind, OldTuple> const & expr, NewTuple && tuple)
    {
        return ExprTemplate<Kind, NewTuple>{std::move(tuple)};
    }

    template<typename Expr, typename Tuple, typename Transform>
    decltype(auto) transform_nonterminal(
        Expr const & expr, Tuple && tuple, Transform & transform)
    {
        auto transformed_tuple = hana::transform(
            static_cast<Tuple &&>(tuple), [&transform](auto && element) {
                using element_t = decltype(element);
                auto const kind = remove_cv_ref_t<element_t>::kind;
                ::boost::yap::detail::transform_impl<
                    false,
                    element_t,
                    Transform,
                    kind == expr_kind::expr_ref>
                    xform;
                return xform(static_cast<element_t &&>(element), transform);
            });
        return make_expr_from_tuple(expr, std::move(transformed_tuple));
    }

    template<>
    struct default_transform<true, false, false>
    {
        template<typename Expr, typename Transform>
        decltype(auto) operator()(Expr && expr, Transform & transform) const
        {
            return transform_nonterminal(expr, expr.elements, transform);
        }
    };

    template<>
    struct default_transform<false, false, false>
    {
        template<typename Expr, typename Transform>
        decltype(auto) operator()(Expr && expr, Transform & transform) const
        {
            return transform_nonterminal(
                expr, std::move(expr.elements), transform);
        }
    };

    // Expression-matching; attempted second.

    template<
        bool Strict,
        typename Expr,
        typename Transform,
        typename = detail::void_t<>>
    struct transform_expression_expr
    {
        decltype(auto) operator()(Expr && expr, Transform & transform)
        {
            // No expr-matching succeeded; use the default transform.
            constexpr expr_kind kind = remove_cv_ref_t<Expr>::kind;
            return default_transform<
                std::is_lvalue_reference<Expr>{},
                kind == expr_kind::terminal,
                Strict>{}(static_cast<Expr &&>(expr), transform);
        }
    };

    template<bool Strict, typename Expr, typename Transform>
    struct transform_expression_expr<
        Strict,
        Expr,
        Transform,
        void_t<decltype(std::declval<Transform &>()(std::declval<Expr>()))>>
    {
        decltype(auto) operator()(Expr && expr, Transform & transform)
        {
            return transform(static_cast<Expr &&>(expr));
        }
    };


    // Tag-matching; attempted first.

    template<
        bool Strict,
        typename Expr,
        typename Transform,
        expr_arity Arity,
        typename>
    struct transform_expression_tag
    {
        decltype(auto) operator()(Expr && expr, Transform & transform)
        {
            // No tag-matching succeeded; try expr-matching.
            return transform_expression_expr<Strict, Expr, Transform>{}(
                static_cast<Expr &&>(expr), transform);
        }
    };

    template<typename T>
    decltype(auto) terminal_value(T && x)
    {
        return value_impl<true>(static_cast<T &&>(x));
    }


    template<bool Strict, typename Expr, typename Transform>
    struct transform_expression_tag<
        Strict,
        Expr,
        Transform,
        expr_arity::one,
        void_t<decltype(std::declval<Transform &>()(
            expr_tag<remove_cv_ref_t<Expr>::kind>{},
            terminal_value(::boost::yap::value(std::declval<Expr>()))))>>
    {
        decltype(auto) operator()(Expr && expr, Transform & transform)
        {
            return transform(
                expr_tag<remove_cv_ref_t<Expr>::kind>{},
                terminal_value(
                    ::boost::yap::value(static_cast<Expr &&>(expr))));
        }
    };

    template<bool Strict, typename Expr, typename Transform>
    struct transform_expression_tag<
        Strict,
        Expr,
        Transform,
        expr_arity::two,
        void_t<decltype(std::declval<Transform &>()(
            expr_tag<remove_cv_ref_t<Expr>::kind>{},
            terminal_value(::boost::yap::left(std::declval<Expr>())),
            terminal_value(::boost::yap::right(std::declval<Expr>()))))>>
    {
        decltype(auto) operator()(Expr && expr, Transform & transform)
        {
            return transform(
                expr_tag<remove_cv_ref_t<Expr>::kind>{},
                terminal_value(::boost::yap::left(static_cast<Expr &&>(expr))),
                terminal_value(
                    ::boost::yap::right(static_cast<Expr &&>(expr))));
        }
    };

    template<bool Strict, typename Expr, typename Transform>
    struct transform_expression_tag<
        Strict,
        Expr,
        Transform,
        expr_arity::three,
        void_t<decltype(std::declval<Transform &>()(
            expr_tag<remove_cv_ref_t<Expr>::kind>{},
            terminal_value(::boost::yap::cond(std::declval<Expr>())),
            terminal_value(::boost::yap::then(std::declval<Expr>())),
            terminal_value(::boost::yap::else_(std::declval<Expr>()))))>>
    {
        decltype(auto) operator()(Expr && expr, Transform & transform)
        {
            return transform(
                expr_tag<remove_cv_ref_t<Expr>::kind>{},
                terminal_value(::boost::yap::cond(static_cast<Expr &&>(expr))),
                terminal_value(::boost::yap::then(static_cast<Expr &&>(expr))),
                terminal_value(
                    ::boost::yap::else_(static_cast<Expr &&>(expr))));
        }
    };

    template<typename Expr, typename Transform>
    struct transform_call_unpacker
    {
        template<long long... I>
        auto operator()(
            Expr && expr,
            Transform & transform,
            std::integer_sequence<long long, I...>)
            -> decltype(transform(
                expr_tag<expr_kind::call>{},
                terminal_value(::boost::yap::get(
                    static_cast<Expr &&>(expr), hana::llong_c<I>))...))
        {
            return transform(
                expr_tag<expr_kind::call>{},
                terminal_value(::boost::yap::get(
                    static_cast<Expr &&>(expr), hana::llong_c<I>))...);
        }
    };

    template<typename Expr>
    constexpr auto indices_for(Expr const & expr)
    {
        constexpr long long size = decltype(hana::size(expr.elements))::value;
        return std::make_integer_sequence<long long, size>();
    }

    template<bool Strict, typename Expr, typename Transform>
    struct transform_expression_tag<
        Strict,
        Expr,
        Transform,
        expr_arity::n,
        void_t<decltype(transform_call_unpacker<Expr, Transform>{}(
            std::declval<Expr>(),
            std::declval<Transform &>(),
            indices_for(std::declval<Expr>())))>>
    {
        decltype(auto) operator()(Expr && expr, Transform & transform)
        {
            return transform_call_unpacker<Expr, Transform>{}(
                static_cast<Expr &&>(expr), transform, indices_for(expr));
        }
    };

    template<bool Strict, typename Expr, typename Transform, bool IsExprRef>
    struct transform_impl
    {
        decltype(auto) operator()(Expr && expr, Transform & transform)
        {
            constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
            return detail::transform_expression_tag<
                Strict,
                Expr,
                Transform,
                detail::arity_of<kind>()>{}(
                static_cast<Expr &&>(expr), transform);
        }
    };

    template<bool Strict, typename Expr, typename Transform>
    struct transform_impl<Strict, Expr, Transform, true>
    {
        decltype(auto) operator()(Expr && expr_, Transform & transform)
        {
            decltype(auto) expr = ::boost::yap::deref(expr_);
            constexpr expr_kind kind =
                detail::remove_cv_ref_t<decltype(expr)>::kind;
            return detail::transform_impl<
                Strict,
                decltype(expr),
                Transform,
                kind == expr_kind::expr_ref>{}(
                static_cast<decltype(expr) &&>(expr), transform);
        }
    };

}}}

#endif
