//[hello_world_redux
#include <boost/yap/algorithm.hpp>

#include <iostream>


template <boost::yap::expr_kind Kind, typename Tuple>
struct stream_expr
{
    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;

    template <typename T>
    decltype(auto) operator<< (T && x)
    { return boost::yap::value(*this) << std::forward<T &&>(x); }
};


int main ()
{
    auto cout = boost::yap::make_terminal<stream_expr>(std::cout);
    cout << "Hello" << ',' << " world!\n";

    return 0;
}
//]
