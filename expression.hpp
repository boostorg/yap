#ifndef BOOST_PROTO17_EXPRESSION_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_HPP_INCLUDED

#include "expression_fwd.hpp"
#include "detail/expression.hpp"
#include "detail/default_eval.hpp"

#include <boost/hana/tuple.hpp>
//#include <boost/hana/type.hpp>
//#include <boost/hana/size.hpp>


namespace boost::proto17 {

    // TODO: Customization point.
    // TODO: static assert/SFINAE std::is_callable<>
    // TODO: static assert/SFINAE no placeholders
    template <typename R, expr_kind Kind, typename ...T>
    R evaluate_expression_as (expression<Kind, T...> const & expr)
    { return static_cast<R>(detail::default_eval_expr(expr, hana::tuple<>())); }

    // TODO: static assert/SFINAE std::is_callable<>
    template <typename Expr, typename ...T>
    auto evaluate (Expr const & expr, T && ...t)
    { return detail::default_eval_expr(expr, hana::make_tuple(static_cast<T &&>(t)...)); }

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
        { return evaluate_expression_as<R>(*this); }

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
