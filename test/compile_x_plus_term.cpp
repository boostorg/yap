#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"

#include <string>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
namespace bh = boost::hana;


void compile_x_plus_term ()
{
    using namespace std::literals;

    // char const * string
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<char const *>,
                term<double>
            >
        > unevaluated_expr = "3" + unity;
        (void)unevaluated_expr;
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<std::string>,
                term<double>
            >
        > unevaluated_expr = "3"s + unity;
        (void)unevaluated_expr;
    }

    // arrays
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int *>,
                term<double>
            >
        > unevaluated_expr = ints + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int const *>,
                term<double>
            >
        > unevaluated_expr = ints + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int *>,
                term<double>
            >
        > unevaluated_expr = std::move(ints) + unity;
        (void)unevaluated_expr;
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int * &>,
                term<double>
            >
        > unevaluated_expr = int_ptr + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int const * &>,
                term<double>
            >
        > unevaluated_expr = int_ptr + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int *>,
                term<double>
            >
        > unevaluated_expr = std::move(int_ptr) + unity;
        (void)unevaluated_expr;
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int * const &>,
                term<double>
            >
        > unevaluated_expr = int_ptr + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int const * const &>,
                term<double>
            >
        > unevaluated_expr = int_ptr + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int * const>,
                term<double>
            >
        > unevaluated_expr = std::move(int_ptr) + unity;
        (void)unevaluated_expr;
    }

    // values
    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int &>,
                term<double>
            >
        > unevaluated_expr = i + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int const i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int const &>,
                term<double>
            >
        > unevaluated_expr = i + unity;
        (void)unevaluated_expr;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            bh::tuple<
                term<int>,
                term<double>
            >
        > unevaluated_expr = std::move(i) + unity;
        (void)unevaluated_expr;
    }
}
