#ifndef BOOST_PROTO17_EXPRESSION_FWD_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_FWD_HPP_INCLUDED


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

}

#endif
