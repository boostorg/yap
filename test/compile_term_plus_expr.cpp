#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
namespace bh = boost::hana;


void compile_term_plus_expr ()
{
    // values
    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int>
            >
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int>
                    >
                >
            >
        > unevaluated_expr = unity + expr;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        term<int const> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int const>
            >
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int const>
                    >
                >
            >
        > unevaluated_expr = unity + expr;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int>
                    >
                >
            >
        > unevaluated_expr = unity + expr;
        (void)unevaluated_expr;
    }

    // const value terminals/expressions
    {
        term<double> unity{1.0};
        term<int> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int>
            >
        > const expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int>
                    >
                >
            >
        > unevaluated_expr = unity + expr;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int>
            >
        > const expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int>
                    >
                >
            >
        > unevaluated_expr = unity + expr;
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
                term<double>,
                term<int &>
            >
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int &>
                    >
                >
            >
        > unevaluated_expr = unity + expr;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int const &>
            >
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int const &>
                    >
                >
            >
        > unevaluated_expr = unity + expr;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int &>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int &>
                    >
                >
            >
        > unevaluated_expr = unity + expr;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int &>
            >
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int &>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int const &>
            >
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int const &>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int &>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int &>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
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
                term<double>,
                term<int &&>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int &&>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int &&>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int &&>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int &&>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int &&>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                term<int &&>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<double>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        term<double>,
                        term<int &&>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }
}
