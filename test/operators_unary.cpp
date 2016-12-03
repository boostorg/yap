#include <boost/yap/expression.hpp>

#include <gtest/gtest.h>

#include <sstream>


template <typename T>
using term = boost::yap::terminal<T>;

template <typename T>
using term_ref = boost::yap::expression_ref<boost::yap::terminal<T> &>;

template <typename T>
using term_cref = boost::yap::expression_ref<boost::yap::terminal<T> const &>;

namespace yap = boost::yap;
namespace bh = boost::hana;


TEST(numeric, operators_unary_test)
{
    term<uint32_t> x{{2u}};
    term<uint32_t> const cx{{3u}};

    {
        yap::expression<
            yap::expr_kind::unary_plus,
            bh::tuple<term<uint32_t>>
        > term_expr = +term<uint32_t>{{1u}};
        yap::expression<
            yap::expr_kind::unary_plus,
            bh::tuple<term_ref<uint32_t>>
        > term_ref_expr = +x;
        yap::expression<
            yap::expr_kind::unary_plus,
            bh::tuple<term_cref<uint32_t>>
        > term_const_ref_expr = +cx;

        EXPECT_EQ(evaluate(term_expr), 1u);
        EXPECT_EQ(evaluate(term_ref_expr), 2u);
        EXPECT_EQ(evaluate(term_const_ref_expr), 3u);
    }

    {
        yap::expression<
            yap::expr_kind::negate,
            bh::tuple<term<uint32_t>>
        > term_expr = -term<uint32_t>{{1u}};
        yap::expression<
            yap::expr_kind::negate,
            bh::tuple<term_ref<uint32_t>>
        > term_ref_expr = -x;
        yap::expression<
            yap::expr_kind::negate,
            bh::tuple<term_cref<uint32_t>>
        > term_const_ref_expr = -cx;

        EXPECT_EQ(evaluate(term_expr), 0u - 1u);
        EXPECT_EQ(evaluate(term_ref_expr), 0u - 2u);
        EXPECT_EQ(evaluate(term_const_ref_expr), 0u - 3u);
    }

    {
        yap::expression<
            yap::expr_kind::complement,
            bh::tuple<term<uint32_t>>
        > term_expr = ~term<uint32_t>{{1u}};
        yap::expression<
            yap::expr_kind::complement,
            bh::tuple<term_ref<uint32_t>>
        > term_ref_expr = ~x;
        yap::expression<
            yap::expr_kind::complement,
            bh::tuple<term_cref<uint32_t>>
        > term_const_ref_expr = ~cx;

        EXPECT_EQ(evaluate(term_expr), 0xfffffffe);
        EXPECT_EQ(evaluate(term_ref_expr), 0xfffffffd);
        EXPECT_EQ(evaluate(term_const_ref_expr), 0xfffffffc);
    }

    {
        yap::expression<
            yap::expr_kind::logical_not,
            bh::tuple<term<uint32_t>>
        > term_expr = !term<uint32_t>{{1u}};
        yap::expression<
            yap::expr_kind::logical_not,
            bh::tuple<term_ref<uint32_t>>
        > term_ref_expr = !x;
        yap::expression<
            yap::expr_kind::logical_not,
            bh::tuple<term_cref<uint32_t>>
        > term_const_ref_expr = !cx;

        EXPECT_EQ(evaluate(term_expr), false);
        EXPECT_EQ(evaluate(term_ref_expr), false);
        EXPECT_EQ(evaluate(term_const_ref_expr), false);
    }

    {
        yap::expression<
            yap::expr_kind::pre_inc,
            bh::tuple<term<uint32_t>>
        > term_expr = ++term<uint32_t>{{1u}};
        yap::expression<
            yap::expr_kind::pre_inc,
            bh::tuple<term_ref<uint32_t>>
        > term_ref_expr = ++x;
        yap::expression<
            yap::expr_kind::pre_inc,
            bh::tuple<term_cref<uint32_t>>
        > term_const_ref_expr = ++cx;
        (void)term_const_ref_expr;

        EXPECT_EQ(evaluate(term_expr), 2u);
        EXPECT_EQ(evaluate(term_ref_expr), 3u);
    }
}

TEST(address_dereference, operators_unary_test)
{
    {
        uint32_t i = 1, j = 2, k = 3;
        term<uint32_t &> x{{j}};
        term<uint32_t &> const cx{{k}};

        yap::expression<
            yap::expr_kind::address_of,
            bh::tuple<term<uint32_t &>>
        > term_expr = &term<uint32_t &>{{i}};
        yap::expression<
            yap::expr_kind::address_of,
            bh::tuple<term_ref<uint32_t &>>
        > term_ref_expr = &x;
        yap::expression<
            yap::expr_kind::address_of,
            bh::tuple<term_cref<uint32_t &>>
        > term_const_ref_expr = &cx;

        EXPECT_EQ(evaluate(term_expr), &i);
        EXPECT_EQ(evaluate(term_ref_expr), &j);
        EXPECT_EQ(evaluate(term_const_ref_expr), &k);
    }

    {
        uint32_t i = 1, j = 2, k = 3;
        term<uint32_t *> x{{&j}};
        term<uint32_t *> const cx{{&k}};

        yap::expression<
            yap::expr_kind::dereference,
            bh::tuple<term<uint32_t *>>
        > term_expr = *term<uint32_t *>{{&i}};
        yap::expression<
            yap::expr_kind::dereference,
            bh::tuple<term_ref<uint32_t *>>
        > term_ref_expr = *x;
        yap::expression<
            yap::expr_kind::dereference,
            bh::tuple<term_cref<uint32_t *>>
        > term_const_ref_expr = *cx;

        EXPECT_EQ(evaluate(term_expr), i);
        EXPECT_EQ(evaluate(term_ref_expr), j);
        EXPECT_EQ(evaluate(term_const_ref_expr), k);
    }
}
