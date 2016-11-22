#ifndef BOOST_PROTO17_EXPRESSION_FWD_HPP_INCLUDED
#define BOOST_PROTO17_EXPRESSION_FWD_HPP_INCLUDED

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>


namespace boost::proto17 {

    enum class expr_kind {
        expr_ref,

        terminal,
        placeholder,

        // unary
        unary_plus, // +
        negate, // -
        dereference, // *
        complement, // ~
        address_of, // &
        logical_not, // !
        pre_inc, // ++
        pre_dec, // --
        post_inc, // ++(int)
        post_dec, // --(int)

        // binary
        shift_left, // <<
        shift_right, // >>
        multiplies, // *
        divides, // /
        modulus, // %
        plus, // +
        minus, // -
        less, // <
        greater, // >
        less_equal, // <=
        greater_equal, // >=
        equal_to, // ==
        not_equal_to, // !=
        logical_or, // ||
        logical_and, // &&
        bitwise_and, // &
        bitwise_or, // |
        bitwise_xor, // ^
        comma, // ,
        mem_ptr, // ->*
        assign, // =
        shift_left_assign, // <<=
        shift_right_assign, // >>=
        multiplies_assign, // *=
        divides_assign, // /=
        modulus_assign, // %=
        plus_assign, // +=
        minus_assign, // -=
        bitwise_and_assign, // &=
        bitwise_or_assign, // |=
        bitwise_xor_assign, // ^=
        subscript, // []

        // n-ary
        call // ()
    };

    template <expr_kind Kind, typename Tuple>
    struct expression;

    template <typename T>
    using terminal = expression<expr_kind::terminal, T>;

    namespace literals {

        template <char ...c>
        constexpr auto operator"" _p ()
        {
            using i = hana::llong<hana::ic_detail::parse<sizeof...(c)>({c...})>;
            static_assert(1 <= i::value, "Placeholders must be >= 1.");
            return expression<expr_kind::placeholder, hana::tuple<i>>(i{});
        }

    }

}

#endif
