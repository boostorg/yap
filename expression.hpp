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

        template <typename U>
        decltype(auto) operator+ (U && rhs) const &
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::plus, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{*this, static_cast<U &&>(rhs)}
            };
        }
        template <typename U>
        decltype(auto) operator+ (U && rhs) &&
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::plus, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{std::move(*this), static_cast<U &&>(rhs)}
            };
        }

        template <typename U>
        decltype(auto) operator- (U && rhs) const &
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::minus, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{*this, static_cast<U &&>(rhs)}
            };
        }
        template <typename U>
        decltype(auto) operator- (U && rhs) &&
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::minus, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{std::move(*this), static_cast<U &&>(rhs)}
            };
        }

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
