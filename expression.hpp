#ifndef BOOST_PROTO17_EXPRESSION_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_HPP_INCLUDED

#include "expression_fwd.hpp"
#include "detail/expression.hpp"
#include "detail/default_eval.hpp"

#include <boost/hana/tuple.hpp>


namespace boost::proto17 {

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr auto evaluate_expression_as (E const & expr, hana::basic_type<R>, T && ...t)
        { return static_cast<R>(detail::default_eval_expr(expr, hana::make_tuple(static_cast<T &&>(t)...))); }

        struct evaluate_expression_as_fn
        {
            template <typename R, typename E, typename ...T>
            constexpr auto operator() (E const & expr, hana::basic_type<R> rtype, T && ...t) const
            { return evaluate_expression_as(expr, rtype, static_cast<T &&>(t)...); }
        };

    }

    using adl_detail::evaluate_expression_as_fn;

    inline namespace function_objects {

        inline constexpr evaluate_expression_as_fn evaluate_expression_as{};

    }

    // TODO: static assert/SFINAE sizeof...(T) >= highest-indexed placeholder + 1
    template <typename Expr, typename ...T>
    auto evaluate (Expr const & expr, T && ...t)
    { return detail::default_eval_expr(expr, hana::make_tuple(static_cast<T &&>(t)...)); }

    // TODO: static assert/SFINAE sizeof...(T) >= highest-indexed placeholder + 1
    template <typename R, typename Expr, typename ...T>
    auto evaluate_as (Expr const & expr, T && ...t)
    { return evaluate_expression_as(expr, hana::basic_type<R>{}, static_cast<T &&>(t)...); }

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
        { return evaluate_expression_as(*this, hana::basic_type<R>{}); }

        template <typename U>
        auto operator+ (U && rhs) const &
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::plus, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{*this, rhs_type{static_cast<U &&>(rhs)}}
            };
        }

        template <typename U>
        auto operator+ (U && rhs) &&
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::plus, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{std::move(*this), rhs_type{static_cast<U &&>(rhs)}}
            };
        }

        template <typename U>
        auto operator- (U && rhs) const &
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::minus, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{*this, rhs_type{static_cast<U &&>(rhs)}}
            };
        }

        template <typename U>
        auto operator- (U && rhs) &&
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::minus, this_type, rhs_type>{
                hana::tuple<this_type, rhs_type>{std::move(*this), rhs_type{static_cast<U &&>(rhs)}}
            };
        }
    };

}

#endif
