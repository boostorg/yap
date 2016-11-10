#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/size.hpp>

#include <algorithm>
#include <iostream>
#include <string>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
using namespace std::string_literals;


void x_plus_term ()
{
#if 0
    // char const * string
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<char const *>,
            term<double>
        > unevaluated_expr = "3" + unity;
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<std::string>,
            term<double>
        > unevaluated_expr = "3"s + unity;
    }

    // arrays
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<int *>,
            term<double>
        > unevaluated_expr = ints + unity;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<int const *>,
            term<double>
        > unevaluated_expr = ints + unity;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<int *>,
            term<double>
        > unevaluated_expr = std::move(ints) + unity;
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int * &>,
            term<double>
        > unevaluated_expr = int_ptr + unity;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int const * &>,
            term<double>
        > unevaluated_expr = int_ptr + unity;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int *>,
            term<double>
        > unevaluated_expr = std::move(int_ptr) + unity;
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int * const &>,
            term<double>
        > unevaluated_expr = int_ptr + unity;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int const * const &>,
            term<double>
        > unevaluated_expr = int_ptr + unity;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int * const>,
            term<double>
        > unevaluated_expr = std::move(int_ptr) + unity;
    }

    // values
    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int &>,
            term<double>
        > unevaluated_expr = i + unity;
    }

    {
        term<double> unity{1.0};
        int const i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int const &>,
            term<double>
        > unevaluated_expr = i + unity;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<int>,
            term<double>
        > unevaluated_expr = std::move(i) + unity;
    }
#endif
}

void term_plus_x ()
{
    // char const * string
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<char const *>
        > unevaluated_expr = unity + "3";
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<std::string>
        > unevaluated_expr = unity + "3"s;
    }

    // arrays
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int *>
        > unevaluated_expr = unity + std::move(ints);
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int * &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const * &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int *>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int * const &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const * const &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int * const>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // values
    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int const i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > unevaluated_expr = unity + std::move(i);
    }
}

void term_plus_x_this_ref_overloads()
{
    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> const unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = std::move(unity) + i;
    }
}

void term_plus_term ()
{
    // char const * string
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<char const *>
        > unevaluated_expr = unity + term<char const *>{"3"};
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<std::string>
        > unevaluated_expr = unity + term<std::string>{"3"s};
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints_[] = {1, 2};
        term<int *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int const ints_[] = {1, 2};
        term<int const *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int ints_[] = {1, 2};
        term<int *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int *>
        > unevaluated_expr = unity + std::move(ints);
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        term<int * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int * const>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        term<int const * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const * const>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        term<int * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int * const>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // values
    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        term<int const> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > unevaluated_expr = unity + std::move(i);
    }

    // const value terminals
    {
        term<double> unity{1.0};
        term<int> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        term<int const> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const>
        > unevaluated_expr = unity + i;
    }

    // lvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + std::move(i);
    }

    // rvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > unevaluated_expr = unity + std::move(i);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > unevaluated_expr = unity + std::move(i);
    }
}

void term_plus_expr ()
{
    // values
    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        term<int const> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int const>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int>
            >
        > unevaluated_expr = unity + expr;
    }

    // const value terminals/expressions
    {
        term<double> unity{1.0};
        term<int> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > const expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > const expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int>
            >
        > unevaluated_expr = unity + expr;
    }

    // lvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int const &>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int const &>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    // rvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }
}

void placeholders ()
{
    using namespace boost::proto17::literals;

    {
        bp17::placeholder<0> p0 = 0_p;
    }

    {
        bp17::placeholder<0> p0 = 0_p;
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<0>,
            term<double>
        > expr = p0 + unity;
    }

    {
        bp17::placeholder<0> p0 = 0_p;
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<0>,
            bp17::placeholder<1>
        > expr = p0 + 1_p;
    }
}

void const_term_expr ()
{
    {
        term<double const> unity{1.0};
        int i_ = 42;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double const>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double const>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double const>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> const unity{1.0};
        int i_ = 42;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 42;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > const expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int const &>
            >
        > unevaluated_expr = unity + std::move(expr);
    }
}

void default_eval ()
{
    std::cout << "\ndefault_eva()\n";

    term<double> unity{1.0};
    int i_ = 42;
    term<int &&> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::minus,
        term<double>,
        term<int &&>
    > expr = unity - std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        bp17::expression<
            bp17::expr_kind::minus,
            term<double>,
            term<int &&>
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        double result = unity;
        std::cout << "unity=" << result << "\n"; // 1
    }

    {
        double result = expr;
        std::cout << "expr=" << result << "\n"; // -41
    }

    {
        double result = unevaluated_expr;
        std::cout << "unevaluated_expr=" << result << "\n"; // -40
    }

    {
        double result = evaluate(unity, 5, 6, 7);
        std::cout << "evaluate(unity)=" << result << "\n"; // 1
    }

    {
        double result = evaluate(expr);
        std::cout << "evaluate(expr)=" << result << "\n"; // -41
    }

    {
        double result = evaluate(unevaluated_expr, std::string("15"));
        std::cout << "evaluate(unevaluated_expr)=" << result << "\n"; // -40
    }

    std::cout << "\n";
}

namespace user {

    struct number
    {
        explicit operator double () const { return value; }

        double value;
    };

    // User-defined binary-plus!  With weird semantics!
    template <typename A, typename B>
    inline auto eval_plus (A a, B b)
    { return number{a.value - b.value}; }

    template <typename E, typename Tuple>
    constexpr auto eval_expression_as (
        E const & expr,
        boost::hana::basic_type<user::number>,
        Tuple && args)
    {
        std::cout << "User eval!  ";
        return static_cast<user::number>(
            bp17::detail::default_eval_expr(expr, static_cast<Tuple &&>(args))
        );
    }

}

void user_operator_eval ()
{
    std::cout << "\nuser_operator_eval()\n";

    term<user::number> unity{{1.0}};
    double d_ = 42.0;
    term<user::number> i{{d_}};
    bp17::expression<
        bp17::expr_kind::plus,
        term<user::number>,
        term<user::number>
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<user::number>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<user::number>,
            term<user::number>
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        double result = unity;
        std::cout << "unity=" << result << "\n"; // 1
    }

    {
        double result = expr;
        std::cout << "expr=" << result << "\n"; // -41
    }

    {
        double result = unevaluated_expr;
        std::cout << "unevaluated_expr=" << result << "\n"; // 42
    }

    {
        double result = (double)evaluate(unity, 5, 6, 7);
        std::cout << "evaluate(unity)=" << result << "\n"; // 1
    }

    {
        double result = (double)evaluate(expr);
        std::cout << "evaluate(expr)=" << result << "\n"; // -41
    }

    {
        double result = (double)evaluate(unevaluated_expr, std::string("15"));
        std::cout << "evaluate(unevaluated_expr)=" << result << "\n"; // 42
    }

    std::cout << "\n";
}

void user_eval_expression_as ()
{
    term<user::number> unity{{1.0}};
    double d_ = 42.0;
    term<user::number> i{{d_}};
    bp17::expression<
        bp17::expr_kind::plus,
        term<user::number>,
        term<user::number>
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<user::number>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<user::number>,
            term<user::number>
        >
    > unevaluated_expr = unity + std::move(expr);

    std::cout << "\nuser_eval_expression_as()\n";

    {
        user::number result = unity;
        std::cout << "unity=" << result.value << "\n"; // 1
    }

    {
        user::number result = expr;
        std::cout << "expr=" << result.value << "\n"; // -41
    }

    {
        user::number result = unevaluated_expr;
        std::cout << "unevaluated_expr=" << result.value << "\n"; // 42
    }

    {
        user::number result = evaluate(unity, 5, 6, 7);
        std::cout << "evaluate(unity)=" << result.value << "\n"; // 1
    }

    {
        double result = (double)evaluate(expr);
        std::cout << "evaluate(expr)=" << result << "\n"; // -41
    }

    {
        double result = (double)evaluate(unevaluated_expr, std::string("15"));
        std::cout << "evaluate(unevaluated_expr)=" << result << "\n"; // 42
    }

    {
        user::number result = bp17::evaluate_as<user::number>(unity, 5, 6, 7);
        std::cout << "evaluate(unity)=" << result.value << "\n"; // 1
    }

    {
        user::number result = bp17::evaluate_as<user::number>(expr);
        std::cout << "evaluate(expr)=" << result.value << "\n"; // -41
    }

    {
        user::number result = bp17::evaluate_as<user::number>(unevaluated_expr, std::string("15"));
        std::cout << "evaluate(unevaluated_expr)=" << result.value << "\n"; // 42
    }

    std::cout << "\n";
}

namespace user_2 {

    struct number
    {
        double value;
    };

    number naxpy (number a, number x, number y)
    { return number{a.value * x.value + y.value}; }

#if 0
    auto eval_expression_as (
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::expression<
                bp17::expr_kind::multiplies,
                term<number>,
                term<number>
            >,
            term<number>
        > const & expr,
        boost::hana::basic_type<number>)
    {
        std::cout << "User naxpy!  ";
        using namespace boost::hana::literals;
        return naxpy(
            expr.elements[0_c].elements[0_c].elements[0_c],
            expr.elements[0_c].elements[1_c].elements[0_c],
            expr.elements[1_c].elements[0_c]
        );
    }
#else
    auto eval_expression_as (
        decltype(term<number>{{0.0}} * number{} + number{}) const & expr,
        boost::hana::basic_type<number>,
        boost::hana::tuple<>)
    {
        std::cout << "User naxpy!  ";
        return naxpy(
            expr.left().left().value(),
            expr.left().right().value(),
            expr.right().value()
        );
    }
#endif

}

void user_expression_transform ()
{
    std::cout << "\nuser_expression_transform()\n";

    term<user_2::number> a{{1.0}};
    term<user_2::number> x{{42.0}};
    term<user_2::number> y{{3.0}};

    bp17::expression<
        bp17::expr_kind::plus,
        bp17::expression<
            bp17::expr_kind::multiplies,
            term<user_2::number>,
            term<user_2::number>
        >,
        term<user_2::number>
    > expr = a * x + y;

    // TODO: This was an error (user:: vs. user_2::).  Document for users that
    // they should catch an expression in an auto var to diagnose these sorts
    // of things.
#if 0
    bp17::expression<
        bp17::expr_kind::multiplies,
        term<user::number>,
        term<user::number>
    > expr = a * x;
#endif

    user_2::number result = expr;

    std::cout << "expr=" << result.value << "\n"; // 45

    std::cout << "\n";
}

void placeholder_eval ()
{
    std::cout << "\nplaceholder_eval()\n";

    using namespace boost::proto17::literals;

    bp17::placeholder<2> p2 = 2_p;
    int i_ = 42;
    term<int> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::plus,
        bp17::placeholder<2>,
        term<int>
    > expr = p2 + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        bp17::placeholder<2>,
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<2>,
            term<int>
        >
    > unevaluated_expr = p2 + std::move(expr);

    {
        double result = evaluate(p2, 5, 6, 7);
        std::cout << "evaluate(p2)=" << result << "\n"; // 7
    }

    {
        double result = evaluate(expr, std::string("15"), 3, 1);
        std::cout << "evaluate(expr)=" << result << "\n"; // 43
    }

    {
        double result = evaluate(unevaluated_expr, std::string("15"), 2, 3);
        std::cout << "evaluate(unevaluated_expr)=" << result << "\n"; // 48
    }

    std::cout << "\n";
}

namespace user {

    inline auto max (int a, int b)
    { return a < b ? b : a; };

    struct tag_type {};

    inline number tag_function (double a, double b)
    { return number{a + b}; }

    // User-defined operator() implementation.
    template <typename F, typename ...T>
    inline auto eval_call (F && f, T && ...t)
    {
        if constexpr (sizeof...(T) == 2u && std::is_same_v<std::decay_t<F>, tag_type>) {
            return tag_function((double)t...);
        } else {
            assert(!"Unhandled case in eval_call()");
            return;
        }
    }

}

void call_expr ()
{
    std::cout << "\ncall_expr()\n";

    using namespace boost::proto17::literals;

    {
        bp17::expression<
            bp17::expr_kind::call,
            bp17::placeholder<0>,
            bp17::placeholder<1>,
            bp17::placeholder<2>
        > expr = 0_p(1_p, 2_p);

        {
            auto min = [] (int a, int b) { return a < b ? a : b; };
            int result = evaluate(expr, min, 3, 7);
            std::cout << "evaluate(expr, min, 3, 7)=" << result << "\n"; // 3
        }

        {
            int result = evaluate(expr, &user::max, 3, 7);
            std::cout << "evaluate(expr, &user::max, 3, 7)=" << result << "\n"; // 7
        }

        {
            int result = evaluate(expr, user::max, 3, 7);
            std::cout << "evaluate(expr, user::max, 3, 7)=" << result << "\n"; // 7
        }
    }

    {
        auto min_lambda = [] (int a, int b) { return a < b ? a : b; };

        {
            auto min = bp17::make_terminal(min_lambda);
            auto expr = min(0_p, 1_p);

            {
                int result = evaluate(expr, 3, 7);
                std::cout << "evaluate(expr, 3, 7)=" << result << "\n"; // 3
            }
        }

        {
            term<decltype(min_lambda)> min = {{min_lambda}};
            auto expr = min(0_p, 1_p);

            {
                int result = evaluate(expr, 3, 7);
                std::cout << "evaluate(expr, 3, 7)=" << result << "\n"; // 3
            }
        }
    }

    {
        struct min_function_object_t
        {
            auto operator() (int a, int b) const { return a < b ? a : b; }
        } min_function_object;

        {
            term<min_function_object_t> min = bp17::make_terminal(min_function_object);
            auto expr = min(0_p, 1_p);

            {
                using namespace boost::hana::literals;
                int result = evaluate(expr, 3, 7);
                std::cout << "evaluate(expr, 3, 7)=" << result << "\n"; // 3
            }
        }

        {
            term<min_function_object_t> min = {{min_function_object}};
            auto expr = min(0_p, 1_p);

            {
                int result = evaluate(expr, 3, 7);
                std::cout << "evaluate(expr, 3, 7)=" << result << "\n"; // 3
            }
        }

        {
            auto min = bp17::make_terminal(min_function_object_t{});
            auto expr = min(0_p, 1_p);

            {
                int result = evaluate(expr, 3, 7);
                std::cout << "evaluate(expr, 3, 7)=" << result << "\n"; // 3
            }
        }

        {
            term<min_function_object_t> min = {{min_function_object_t{}}};
            auto expr = min(0_p, 1_p);

            {
                int result = evaluate(expr, 3, 7);
                std::cout << "evaluate(expr, 3, 7)=" << result << "\n"; // 3
            }
        }

    }

    {
        auto min_lambda = [] (int a, int b) { return a < b ? a : b; };

        {
            auto min = bp17::make_terminal(min_lambda);
            auto expr = min(0, 1);

            {
                int result = evaluate(expr);
                std::cout << "evaluate(expr)=" << result << "\n"; // 0
            }
        }

        {
            term<decltype(min_lambda)> min = {{min_lambda}};
            bp17::expression<
                bp17::expr_kind::call,
                term<decltype(min_lambda)>,
                term<int>,
                term<int>
            > expr = min(0, 1);

            {
                int result = evaluate(expr);
                std::cout << "evaluate(expr)=" << result << "\n"; // 0
            }
        }
    }

    {
        {
            auto plus = bp17::make_terminal(user::tag_type{});
            auto expr = plus(user::number{13}, 1);

            {
                user::number result = expr;
                std::cout << "expr=" << result.value << "\n"; // 42
            }
        }

        {
            term<user::tag_type> plus = {{user::tag_type{}}};
            bp17::expression<
                bp17::expr_kind::call,
                term<user::tag_type>,
                term<int>,
                term<int>
            > expr = plus(0, 1);

            {
                user::number result = expr;
                std::cout << "expr=" << result.value << "\n"; // 0
            }
        }
    }

    std::cout << "\n";
}

namespace reference_returning {

    struct number
    {
        double value;
    };

    number const the_result{13.0};

    // User-defined binary-plus!  With weird reference semantics!
    template <typename A, typename B>
    inline number const & eval_plus (A a, B b)
    { return the_result; }

}

void reference_returns ()
{
    term<reference_returning::number> unity = {{1.0}};
    auto expr = unity + reference_returning::number{1.0};

    // TODO: This has to fail due to the general implicit declaration rules
    // (see [conv] 3-6).  This needs to be noted in docs.
#if 0
    {
        reference_returning::number const & n = expr;
        if (&n == &reference_returning::the_result)
            std::cout << "Got the_result.\n";
        else
            std::cout << "FAIL\n";
    }
#endif

    {
        reference_returning::number const & n = evaluate(expr);
        if (&n == &reference_returning::the_result)
            std::cout << "Got the_result.\n";
        else
            std::cout << "FAIL\n";
    }

    static_assert(
        std::is_same_v<
            decltype(evaluate(expr)),
            reference_returning::number const &
        >
    );
}

void expression_depth_stress_right ()
{
    term<double> unity{1.0};

    {
        auto expr = unity + unity + unity + unity +    unity + unity + unity + unity;
        double result = expr;
    }

    {
        auto expr =
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +

            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity;
        double result = expr;
    }

    {
        auto expr =
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +

            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity +
            unity + unity + unity + unity +    unity + unity + unity + unity;
        double result =
            expr + expr + expr + expr +    expr + expr + expr + expr +
            expr + expr + expr + expr +    expr + expr + expr + expr +
            expr + expr + expr + expr +    expr + expr + expr + expr +
            expr + expr + expr + expr +    expr + expr + expr + expr +

            expr + expr + expr + expr +    expr + expr + expr + expr +
            expr + expr + expr + expr +    expr + expr + expr + expr +
            expr + expr + expr + expr +    expr + expr + expr + expr +
            expr + expr + expr + expr +    expr + expr + expr + expr;
    }
}

void expression_depth_stress_left ()
{
    term<double> unity{1.0};

    {
        auto expr = (unity + (unity + (unity + (unity +    (unity + (unity + (unity + unity)))))));
        double result = expr;
    }

    {
        auto expr =
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +

            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
        double result = expr;
    }

    {
        auto expr =
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +

            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity +
            (unity + (unity + (unity + (unity +    (unity + (unity + (unity + (unity))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
        double result =
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +

            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr +
            (expr + (expr + (expr + (expr +    (expr + (expr + (expr + (expr))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))))));
    }
}

int main ()
{
    term_plus_x();
    term_plus_x_this_ref_overloads();
    term_plus_term();
    term_plus_expr();
    placeholders();

    const_term_expr();

    default_eval();
    user_eval_expression_as();
    user_operator_eval();
    user_expression_transform();

    placeholder_eval();

    call_expr();

    reference_returns();
}
