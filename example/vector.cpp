#include "expression.hpp"

#include <vector>
#include <iostream>


struct take_nth
{
    template <typename T>
    auto operator() (boost::yap::terminal_tag, std::vector<T> const & vec)
    { return boost::yap::make_terminal(std::move(vec[n])); }

    std::size_t n;
};

template <typename T>
using vector_terminal = boost::yap::expression<
    boost::yap::expr_kind::terminal,
    boost::hana::tuple<std::vector<T>>
>;

struct equal_sizes_impl
{
    template <typename T>
    auto operator() (vector_terminal<T> const & expr)
    {
        auto const expr_size = boost::yap::value(expr).size();
        if (expr_size != size)
            value = false;
        return expr;
    }

    std::size_t const size;
    bool value;
};

template <typename Expr>
bool equal_sizes (std::size_t size, Expr const & expr)
{
    equal_sizes_impl impl{size, true};
    boost::yap::transform(expr, impl);
    return impl.value;
}


template <typename T, typename Expr>
std::vector<T> & assign (std::vector<T> & vec, Expr const & e)
{
    decltype(auto) expr = boost::yap::as_expr(e);
    assert(equal_sizes(vec.size(), expr));
    for (std::size_t i = 0, size = vec.size(); i < size; ++i) {
        vec[i] = boost::yap::evaluate(boost::yap::transform(expr, take_nth{i}));
    }
    return vec;
}

template <typename T, typename Expr>
std::vector<T> & operator+= (std::vector<T> & vec, Expr const & e)
{
    decltype(auto) expr = boost::yap::as_expr(e);
    assert(equal_sizes(vec.size(), expr));
    for (std::size_t i = 0, size = vec.size(); i < size; ++i) {
        vec[i] += boost::yap::evaluate(boost::yap::transform(expr, take_nth{i}));
    }
    return vec;
}

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(negate, boost::yap::expression, is_vector); // -
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(multiplies, boost::yap::expression, is_vector); // *
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(divides, boost::yap::expression, is_vector); // /
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(modulus, boost::yap::expression, is_vector); // %
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(plus, boost::yap::expression, is_vector); // +
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(minus, boost::yap::expression, is_vector); // -
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(less, boost::yap::expression, is_vector); // <
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(greater, boost::yap::expression, is_vector); // >
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(less_equal, boost::yap::expression, is_vector); // <=
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(greater_equal, boost::yap::expression, is_vector); // >=
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(equal_to, boost::yap::expression, is_vector); // ==
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(not_equal_to, boost::yap::expression, is_vector); // !=
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(logical_or, boost::yap::expression, is_vector); // ||
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(logical_and, boost::yap::expression, is_vector); // &&
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(bitwise_and, boost::yap::expression, is_vector); // &
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(bitwise_or, boost::yap::expression, is_vector); // |
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(bitwise_xor, boost::yap::expression, is_vector); // ^

int main()
{
    int i;
    int const n = 10;
    std::vector<int> a,b,c,d;
    std::vector<double> e(n);

    for (i = 0; i < n; ++i)
    {
        a.push_back(i);
        b.push_back(2*i);
        c.push_back(3*i);
        d.push_back(i);
    }

    assign(b, 2);
    assign(d, a + b * c);

    if_else(d < 30, b, c);
    a += if_else(d < 30, b, c);

    assign(e, c);
    e += e - 4 / (c + 1);

    for (i = 0; i < n; ++i)
    {
        std::cout
            << " a(" << i << ") = " << a[i]
            << " b(" << i << ") = " << b[i]
            << " c(" << i << ") = " << c[i]
            << " d(" << i << ") = " << d[i]
            << " e(" << i << ") = " << e[i]
            << std::endl;
    }

    return 0;
}
