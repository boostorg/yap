#include "expression.hpp"

template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
namespace bh = boost::hana;


void compile_const_term ()
{
    {
        term<double const> unity{1.0};
        int i_ = 42;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double const> &>,
                term<int &&>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double const> &>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        bp17::expression_ref<term<double const> &>,
                        term<int &&>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }

    {
        term<double> const unity{1.0};
        int i_ = 42;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> const &>,
                term<int &&>
            >
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> const &>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        bp17::expression_ref<term<double> const &>,
                        term<int &&>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 42;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                term<int const &>
            >
        > const expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                bp17::expression_ref<term<double> &>,
                bp17::expression<
                    bp17::expr_kind::plus,
                    bh::tuple<
                        bp17::expression_ref<term<double> &>,
                        term<int const &>
                    >
                >
            >
        > unevaluated_expr = unity + std::move(expr);
        (void)unevaluated_expr;
    }
}
