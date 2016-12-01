#include "expression.hpp"

#include <iostream>


int main ()
{
    evaluate(boost::yap::make_terminal(std::cout) << "Hello" << ',' << " world!\n");

    return 0;
}
