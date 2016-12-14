#include <boost/yap/yap.hpp>

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

//[ eval_plus_example_decls
namespace user {

    struct number
    {
        double value;
    };

    number eval_plus (number lhs, number rhs)
    { return number{lhs.value + rhs.value}; }

}
//]

//[ transform_expression_example_decls
// A convenient alias; nothing to see here.
template <typename T>
using term = boost::yap::terminal<boost::yap::expression, T>;

namespace user {

    auto transform_expression (decltype(term<number>{{0.0}} * number{}) const & expr)
    { return "Yay."; }

}
//]

int main ()
{
    foo();

    {
//[ eval_plus_example_use
    auto plus_expr = boost::yap::make_terminal(user::number{2.0}) + user::number{1.0};
    auto minus_expr = boost::yap::make_terminal(user::number{2.0}) - user::number{1.0};

    user::number plus_result = evaluate(plus_expr);
    std::cout << plus_result.value << "\n"; // Prints 1.

    // Does not compile; there is no operator- defined over user::number.
    //user::number minus_result = evaluate(minus_expr);
//]
    }

    {
//[ transform_expression_example_use
    auto expr = term<user::number>{{1.0e6}} * user::number{1.0e6};
    std::cout << evaluate(expr) << "\n"; // Prints "Yay."  Wierd!
//]
    }

    return 0;
}
