#include "expression.hpp"

#include <string>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
namespace bh = boost::hana;


void compile_term_plus_term ()
{
    using namespace std::literals;

    // char const * string
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<char const *>
            >
        > unevaluated_expr = unity + term<char const *>{"3"};
        (void)unevaluated_expr;
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<std::string>
            >
        > unevaluated_expr = unity + term<std::string>{"3"s};
        (void)unevaluated_expr;
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints_[] = {1, 2};
        term<int *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int *> &>
            >
        > unevaluated_expr = unity + ints;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const ints_[] = {1, 2};
        term<int const *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int const *> &>
            >
        > unevaluated_expr = unity + ints;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int ints_[] = {1, 2};
        term<int *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int *>
            >
        > unevaluated_expr = unity + std::move(ints);
        (void)unevaluated_expr;
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        term<int * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int * const> &>
            >
        > unevaluated_expr = unity + int_ptr;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        term<int const * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int const * const> &>
            >
        > unevaluated_expr = unity + int_ptr;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        term<int * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int * const>
            >
        > unevaluated_expr = unity + std::move(int_ptr);
        (void)unevaluated_expr;
    }

    // values
    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int> &>
            >
        > unevaluated_expr = unity + i;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        term<int const> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int const> &>
            >
        > unevaluated_expr = unity + i;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int>
            >
        > unevaluated_expr = unity + std::move(i);
        (void)unevaluated_expr;
    }

    // const value terminals
    {
        term<double> unity{1.0};
        term<int> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int> const &>
            >
        > unevaluated_expr = unity + i;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        term<int const> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int const> const &>
            >
        > unevaluated_expr = unity + i;
        (void)unevaluated_expr;
    }

    // lvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int &> &>
            >
        > unevaluated_expr = unity + i;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression_ref<term<int const &> &>
            >
        > unevaluated_expr = unity + i;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int &>
            >
        > unevaluated_expr = unity + std::move(i);
        (void)unevaluated_expr;
    }

    // rvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(i);
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(i);
        (void)unevaluated_expr;
    }
}
