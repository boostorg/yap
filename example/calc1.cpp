#include "expression.hpp"

#include <iostream>


int main ()
{
    using namespace boost::yap::literals;

    // Displays "5"
    std::cout << evaluate( 1_p + 2.0, 3.0 ) << std::endl;

    // Displays "6"
    std::cout << evaluate( 1_p * 2_p, 3.0, 2.0 ) << std::endl;

    // Displays "0.5"
    std::cout << evaluate( (1_p - 2_p) / 2_p, 3.0, 2.0 ) << std::endl;

    return 0;
}
