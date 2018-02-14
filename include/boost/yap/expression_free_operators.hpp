#ifndef BOOST_YAP_EXPRESSION_FREE_OPERATORS_HPP_INCLUDED
#define BOOST_YAP_EXPRESSION_FREE_OPERATORS_HPP_INCLUDED


namespace boost { namespace yap {

#ifndef BOOST_YAP_DOXYGEN

#define BOOST_YAP_BINARY_FREE_OPERATOR(op_name)                     \
    BOOST_YAP_USER_FREE_BINARY_OPERATOR(op_name, expression)

    BOOST_YAP_BINARY_FREE_OPERATOR(shift_left) // <<
    BOOST_YAP_BINARY_FREE_OPERATOR(shift_right) // >>
    BOOST_YAP_BINARY_FREE_OPERATOR(multiplies) // *
    BOOST_YAP_BINARY_FREE_OPERATOR(divides) // /
    BOOST_YAP_BINARY_FREE_OPERATOR(modulus) // %
    BOOST_YAP_BINARY_FREE_OPERATOR(plus) // +
    BOOST_YAP_BINARY_FREE_OPERATOR(minus) // -
    BOOST_YAP_BINARY_FREE_OPERATOR(less) // <
    BOOST_YAP_BINARY_FREE_OPERATOR(greater) // >
    BOOST_YAP_BINARY_FREE_OPERATOR(less_equal) // <=
    BOOST_YAP_BINARY_FREE_OPERATOR(greater_equal) // >=
    BOOST_YAP_BINARY_FREE_OPERATOR(equal_to) // ==
    BOOST_YAP_BINARY_FREE_OPERATOR(not_equal_to) // !=
    BOOST_YAP_BINARY_FREE_OPERATOR(logical_or) // ||
    BOOST_YAP_BINARY_FREE_OPERATOR(logical_and) // &&
    BOOST_YAP_BINARY_FREE_OPERATOR(bitwise_and) // &
    BOOST_YAP_BINARY_FREE_OPERATOR(bitwise_or) // |
    BOOST_YAP_BINARY_FREE_OPERATOR(bitwise_xor) // ^

#undef BOOST_YAP_BINARY_FREE_OPERATOR

#else

    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator<< (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator>> (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator* (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator/ (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator% (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator+ (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator- (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator< (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator> (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator<= (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator>= (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator== (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator!= (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator|| (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator&& (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator& (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator| (T && lhs, Expr && rhs);
    /** \see BOOST_YAP_USER_FREE_BINARY_OPERATOR for full semantics. */
    template <typename T, typename Expr> auto operator^ (T && lhs, Expr && rhs);

#endif // BOOST_YAP_DOXYGEN

} }

#endif
