#include <boost/yap/yap.hpp>
#include <boost/yap/print.hpp>

#include <iostream>


void foo ()
{
//[ assign_through_terminal
    int i = 0;
    auto expr = boost::yap::make_terminal(i) = 42;
    evaluate(expr);
    std::cout << i << "\n"; // Prints 42.
//]
}

//[ print_decl
struct thing {};
//]

void print_expr ()
{
//[ print_expr
using namespace boost::yap::literals;

auto const const_lvalue_terminal_containing_rvalue = boost::yap::make_terminal("lvalue terminal");

double const d = 1.0;
auto rvalue_terminal_containing_lvalue = boost::yap::make_terminal(d);

auto thing_terminal = boost::yap::make_terminal(thing{});

auto expr =
    4_p +
    std::move(rvalue_terminal_containing_lvalue) * thing_terminal -
    const_lvalue_terminal_containing_rvalue;
//]

boost::yap::print(std::cout, expr) << "\n";
}

int main ()
{
    foo();

    print_expr();

    return 0;
}
