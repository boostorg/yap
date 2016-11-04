#ifndef BOOST_PROTO17_EXPRESSION_FWD_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_FWD_HPP_INCLUDED

#include <boost/hana/fwd/integral_constant.hpp>


namespace boost::proto17 {

    enum class expr_kind {
        terminal,
        placeholder,

        plus,
        minus,

        // etc...
    };

    template <expr_kind Kind, typename ...T>
    struct expression;

    template <typename T>
    using terminal = expression<expr_kind::terminal, T>;

    template <long long I>
    using placeholder = expression<expr_kind::placeholder, hana::llong<I>>;

}

#endif
