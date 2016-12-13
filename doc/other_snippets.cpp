#include <boost/yap/expression.hpp>

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

int main ()
{
    foo();

    return 0;
}
