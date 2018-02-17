// Copyright (C) 2016-2018 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//[ terminal_transforms
#include <boost/yap/yap.hpp>
#include <boost/yap/print.hpp> // TODO

#include <vector>
#include <iostream>


//[ vector_take_nth_xform
struct iota_terminal_transform
{
    // Base case. Note that we're ignoring placeholders entirely for this
    // example (they're easy to special-case if necessary).
    template<typename T>
    auto operator()(boost::yap::expr_tag<boost::yap::expr_kind::terminal>, T && t)
    {
        // Like the std::iota() algorithm, we create terminals in the sequence
        // index_, index_ + 1, index_ + 2, etc.
        return boost::yap::make_terminal(index_++);
    }

    // Recursive case: Match any binary expression.
    template<boost::yap::expr_kind Kind, typename LExpr, typename RExpr>
    auto operator()(boost::yap::expr_tag<Kind>, LExpr const & left, RExpr const & right)
    {
        // Since we need an expr_kind and not a tag-type to make an
        // expression, we use yap::to_kind<>() to covnert one to the other.
        return boost::yap::make_expression<Kind>(
            boost::yap::transform(boost::yap::as_expr(left), *this),
            boost::yap::transform(boost::yap::as_expr(right), *this));
    }

    // Recursive case: Match any unary expression.
    template<boost::yap::expr_kind Kind, typename Expr>
    auto operator()(boost::yap::expr_tag<Kind>, Expr const & expr)
    {
        return boost::yap::make_expression<Kind>(
            boost::yap::transform(boost::yap::as_expr(expr), *this));
    }

#if 1
    auto operator()(boost::yap::expr_tag<boost::yap::expr_kind::call>,
                    int (*callable)(int, int), int a, int b)
    {
        std::cout << "====================\n"
                  << "boost::yap::value(callable)=" << boost::yap::value(callable) << "\n";
        // Even though the callable in a call expression is technically a
        // terminal, it doesn't make a lot of sense to replace it with an int,
        // so we'll only transform the argument subexpressions.
        return boost::yap::make_expression<boost::yap::expr_kind::call>(
            boost::yap::as_expr(callable),
            boost::yap::transform(boost::yap::as_expr(a), *this),
            boost::yap::transform(boost::yap::as_expr(b), *this));
    }
#else // TODO: Is not picked up; needs a fix.
    // Recursive case: Match any call expression.
    template<boost::yap::expr_kind Kind, typename CallableExpr, typename... Arg>
    auto operator()(boost::yap::expr_tag<boost::yap::expr_kind::call>,
                    CallableExpr && callable, Arg &&... arg)
    {
        std::cout << "++++++++++++++++++++\n"
                  << "boost::yap::value(callable)=" << boost::yap::value(callable) << "\n";
        // Even though the callable in a call expression is technically a
        // terminal, it doesn't make a lot of sense to replace it with an int,
        // so we'll only transform the argument subexpressions.
        return boost::yap::make_expression<boost::yap::expr_kind::call>(
            std::forward<CallableExpr>(callable),
            boost::yap::transform(boost::yap::as_expr(arg), *this)...);
    }
#endif

    // if_else/ternary added as necessary.

    int index_;
};
//]

int sum(int a, int b) { return a + b; }

int main()
{
    {
        auto expr = boost::yap::make_terminal(sum)(8, 8);
        assert(evaluate(expr) == 16);

        auto iota_expr = boost::yap::transform(expr, iota_terminal_transform{1});
        assert(evaluate(iota_expr) == 3);
    }

    {
        auto expr = -(boost::yap::make_terminal(8) + 8);
        assert(evaluate(expr) == -16);

        auto iota_expr = boost::yap::transform(expr, iota_terminal_transform{0});
        assert(evaluate(iota_expr) == -1);
    }
}
//]
