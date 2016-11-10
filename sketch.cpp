#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

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

int main ()
{
    term_plus_x();
    term_plus_x_this_ref_overloads();
    term_plus_term();
    term_plus_expr();
    placeholders();

    const_term_expr();
}
