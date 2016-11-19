#include "expression.hpp"

#include <iostream>


int main ()
{
    evaluate(boost::proto17::make_terminal(std::cout) << "Hello" << ',' << " world!\n");

    return 0;
}
