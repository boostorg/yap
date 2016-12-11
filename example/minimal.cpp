#include <boost/yap/expression.hpp>

#include <array>
#include <iostream>


//[ minimal_template
template <boost::yap::expr_kind Kind, typename Tuple>
struct minimal_expr
{
    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;
};
//]


int main()
{
//[ minimal_template_manual_construction
    auto left = boost::yap::make_terminal<minimal_expr>(1);
    auto right = boost::yap::make_terminal<minimal_expr>(41);

    auto expr = boost::yap::make_expression<
        minimal_expr,
        boost::yap::expr_kind::plus
    >(left, right);
//]

//[ minimal_template_evaluation
    auto result = boost::yap::evaluate(expr);

    std::cout << result << "\n"; // prints "42"
//]

    return 0;
}
