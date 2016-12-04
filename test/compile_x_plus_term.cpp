#include <boost/yap/expression.hpp>
#include <boost/yap/expression_free_operators.hpp>

#include <string>


template <typename T>
using term = boost::yap::terminal<T>;

namespace yap = boost::yap;
namespace bh = boost::hana;


void compile_x_plus_term ()
{
    using namespace std::literals;

    // char const * string
    {
        term<double> unity{1.0};
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<char const *>,
                yap::expression_ref <term<double>&>
            >
        > unevaluated_expr = "3" + unity;
        (void)unevaluated_expr;
    }

    // std::string temporary
    {
        term<double> const unity{1.0};
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<std::string>,
                yap::expression_ref<term<double> const &>
            >
        > unevaluated_expr = "3"s + unity;
        (void)unevaluated_expr;
    }

    // arrays
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int *>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = ints + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int const *>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = ints + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int *>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = std::move(ints) + unity;
        (void)unevaluated_expr;
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int * &>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = int_ptr + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * int_ptr = ints;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int const * &>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = int_ptr + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int *>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = std::move(int_ptr) + unity;
        (void)unevaluated_expr;
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int * const &>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = int_ptr + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * const int_ptr = ints;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int const * const &>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = int_ptr + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int * const>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = std::move(int_ptr) + unity;
        (void)unevaluated_expr;
    }

    // values
    {
        term<double> unity{1.0};
        int i = 1;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int &>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = i + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const i = 1;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int const &>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = i + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        yap::expression<
            yap::expr_kind::plus,
            bh::tuple<
                term<int>,
                yap::expression_ref<term<double> &>
            >
        > unevaluated_expr = std::move(i) + unity;
        (void)unevaluated_expr;
    }
}
