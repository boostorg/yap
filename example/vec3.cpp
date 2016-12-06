#include <boost/yap/yap.hpp>

#include <array>
#include <iostream>


struct take_nth
{
    auto operator() (boost::yap::terminal<boost::yap::expression, std::array<int, 3>> const & expr)
    {
        int x = boost::yap::value(expr)[n];
        return boost::yap::make_terminal(std::move(x));
    }

    std::size_t n;
};

using vec3_terminal = boost::yap::expression<
    boost::yap::expr_kind::terminal,
    boost::hana::tuple<std::array<int, 3>>
>;

struct vec3 : vec3_terminal
{
    explicit vec3 (int i = 0, int j = 0, int k = 0)
    {
        (*this)[0] = i;
        (*this)[1] = j;
        (*this)[2] = k;
    }

    explicit vec3 (std::array<int, 3> a)
    {
        (*this)[0] = a[0];
        (*this)[1] = a[1];
        (*this)[2] = a[2];
    }

    int & operator[] (std::ptrdiff_t i)
    { return boost::yap::value(*this)[i]; }

    int const & operator[] (std::ptrdiff_t i) const
    { return boost::yap::value(*this)[i]; }

    template <typename T>
    vec3 & operator= (T const & t)
    {
        decltype(auto) expr = boost::yap::as_expr(t);
        (*this)[0] = boost::yap::evaluate(boost::yap::transform(expr, take_nth{0}));
        (*this)[1] = boost::yap::evaluate(boost::yap::transform(expr, take_nth{1}));
        (*this)[2] = boost::yap::evaluate(boost::yap::transform(expr, take_nth{2}));
        return *this;
    }

    void print() const
    {
        std::cout << '{' << (*this)[0]
                  << ", " << (*this)[1]
                  << ", " << (*this)[2]
                  << '}' << std::endl;
    }
};

struct count_leaves_impl
{
    auto operator() (vec3_terminal const & expr)
    {
        value += 1;
        return expr;
    }

    int value = 0;
};

template <typename Expr>
int count_leaves (Expr const & expr)
{
    count_leaves_impl impl;
    boost::yap::transform(expr, impl);
    return impl.value;
}


int main()
{
    vec3 a, b, c;

    c = 4;

    b[0] = -1;
    b[1] = -2;
    b[2] = -3;

    a = b + c;

    a.print();

    vec3 d;
    auto expr1 = b + c;
    d = expr1;
    d.print();

    int num = count_leaves(expr1);
    std::cout << num << std::endl;

    num = count_leaves(b + 3 * c);
    std::cout << num << std::endl;

    num = count_leaves(b + c * d);
    std::cout << num << std::endl;

    return 0;
}
