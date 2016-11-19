#include "expression.hpp"

#include <iostream>


int main ()
{
    using namespace boost::proto17::literals;

    // Displays "5"
    std::cout << make_expression_function(1_p + 2.0)(3.0) << std::endl;

    // Displays "6"
    std::cout << make_expression_function(1_p * 2_p)(3.0, 2.0) << std::endl;

    // Displays "0.5"
    std::cout << make_expression_function((1_p - 2_p) / 2_p)(3.0, 2.0) << std::endl;

    return 0;
}
