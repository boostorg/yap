// Copyright (C) 2016-2018 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/yap/yap.hpp>

#include <boost/hana/map.hpp>
#include <boost/hana/at_key.hpp>
#include <boost/hana/contains.hpp>
#include <boost/hana/keys.hpp>

#include <vector>
#include <iostream>


template<long long I>
struct let_placeholder : boost::hana::llong<I>
{
};

template<typename ExprMap>
struct let_terminal_transform
{
    template<typename T>
    auto
    operator()(boost::yap::expr_tag<boost::yap::expr_kind::terminal>, T && t)
    {
        return boost::yap::as_expr(std::forward<T>(t));
    }

    template<long long I>
    auto operator()(
        boost::yap::expr_tag<boost::yap::expr_kind::terminal>,
        let_placeholder<I> i)
    {
        if constexpr (boost::hana::contains(
                          decltype(boost::hana::keys(map_))(),
                          boost::hana::llong_c<I>)) {
            return map_[boost::hana::llong_c<I>];
        } else {
            return boost::yap::make_terminal(i);
        }
    }

    template<boost::yap::expr_kind Kind, typename... Args>
    auto operator()(boost::yap::expr_tag<Kind>, Args &&... args)
    {
        return boost::yap::make_expression<Kind>(
            boost::yap::transform(args, *this)...);
    }

    template<typename CallableExpr, typename... Arg>
    auto operator()(
        boost::yap::expr_tag<boost::yap::expr_kind::call>,
        CallableExpr callable,
        Arg &&... arg)
    {
        return boost::yap::make_expression<boost::yap::expr_kind::call>(
            callable, boost::yap::transform(arg, *this)...);
    }

    ExprMap map_;
};

template<typename ExprMap>
struct let_result
{
    template<typename Expr>
    auto operator[](Expr && expr)
    {
        return boost::yap::transform(
            std::forward<Expr>(expr), let_terminal_transform<ExprMap>{map_});
    }

    ExprMap map_;
};

template<typename Map, typename Expr, typename... Exprs>
auto let_impl(Map && map, Expr && expr, Exprs &&... exprs)
{
    static_assert(
        Expr::kind == boost::yap::expr_kind::assign,
        "Expressions passed to let() must be of the form placeholder = Expression");
    if constexpr (sizeof...(Exprs) == 0) {
        using I = typename std::remove_reference<decltype(
            boost::yap::value(boost::yap::left(expr)))>::type;
        auto const i = boost::hana::llong_c<I::value>;
        using map_t = decltype(boost::hana::insert(
            map, boost::hana::make_pair(i, boost::yap::right(expr))));
        return let_result<map_t>{boost::hana::insert(
            map, boost::hana::make_pair(i, boost::yap::right(expr)))};
    } else {
        using I = typename std::remove_reference<decltype(
            boost::yap::value(boost::yap::left(expr)))>::type;
        auto const i = boost::hana::llong_c<I::value>;
        return let_impl(
            boost::hana::insert(
                map, boost::hana::make_pair(i, boost::yap::right(expr))),
            std::forward<Exprs>(exprs)...);
    }
}

template<typename Expr, typename... Exprs>
auto let(Expr && expr, Exprs &&... exprs)
{
    return let_impl(
        boost::hana::make_map(),
        std::forward<Expr>(expr),
        std::forward<Exprs>(exprs)...);
}

int main()
{
    boost::yap::expression<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<let_placeholder<0>>> const _a;
    boost::yap::expression<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<let_placeholder<1>>> const _b;
    auto const cout = boost::yap::make_terminal(std::cout);

    using namespace boost::yap::literals;

    {
        auto expr = let(_a = 2)[_a + 1];
        assert(boost::yap::evaluate(expr) == 3);
    }

    {
        auto expr = let(_a = 123, _b = 456)[_a + _b];
        assert(boost::yap::evaluate(expr) == 123 + 456);
    }

    {
        int i = 1;

        boost::yap::evaluate(let(_a = 1_p)[cout << --_a << ' '], i);

        std::cout << i << std::endl;
    }

    {
        boost::yap::evaluate(
            let(_a = 1_p, _b = 2_p)
            [
                // _a here is an int: 1

               let(_a = 3_p) // hides the outer _a
               [
                   cout << _a << _b // prints "Hello, World"
               ]
            ],
            1, " World", "Hello,"
        );
    }
}
