#ifndef BOOST_YAP_EXPRESSION_HPP_INCLUDED
#define BOOST_YAP_EXPRESSION_HPP_INCLUDED

#include <boost/yap/algorithm.hpp>


namespace boost { namespace yap {

    /** Reference expression template that provides all operator overloads.

        \note Due to a limitation of Doxygen, each of the
        <code>value()</code>, <code>left()</code>, <code>right()</code>, and
        operator overloads listed here is a stand-in for three member
        functions.  For each function <code>f</code>, the listing here is:
        \code return_type f (); \endcode  However, there are actually three
        functions:
        \code
        return_type f () const &;
        return_type f () &;
        return_type f () &&;
        \endcode
    */
    template <expr_kind Kind, typename Tuple>
    struct expression
    {
        using tuple_type = Tuple;

        static const expr_kind kind = Kind;

        /** Default constructor.  Does nothing. */
        expression () {}

        /** Moves \a rhs into the only data mamber, \c elements. */
        expression (tuple_type && rhs) :
            elements (std::move(rhs))
        {}

        tuple_type elements;

        /** A convenience member function that dispatches to the free function
            <code>value()</code>. */
        decltype(auto) value () &
        { return ::boost::yap::value(*this); }

#ifndef BOOST_YAP_DOXYGEN

        decltype(auto) value () const &
        { return ::boost::yap::value(*this); }

        decltype(auto) value () &&
        { return ::boost::yap::value(std::move(*this)); }

#endif

        /** A convenience member function that dispatches to the free function
            <code>left()</code>. */
        decltype(auto) left () &
        { return ::boost::yap::left(*this); }

#ifndef BOOST_YAP_DOXYGEN

        decltype(auto) left () const &
        { return ::boost::yap::left(*this); }

        decltype(auto) left () &&
        { return ::boost::yap::left(std::move(*this)); }

#endif

        /** A convenience member function that dispatches to the free function
            <code>right()</code>. */
        decltype(auto) right () &
        { return ::boost::yap::right(*this); }

#ifndef BOOST_YAP_DOXYGEN

        decltype(auto) right () const &
        { return ::boost::yap::right(*this); }

        decltype(auto) right () &&
        { return ::boost::yap::right(std::move(*this)); }

#endif

#ifdef BOOST_YAP_DOXYGEN

        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator+ ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator- ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator* ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator~ ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator& ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator! ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator++ ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator-- ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator++ (int);
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator-- (int);

        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator<< (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator>> (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator* (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator/ (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator% (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator+ (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator- (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator< (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator> (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator<= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator>= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator== (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator!= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator|| (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator&& (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator& (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator| (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator^ (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator, (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator->* (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator<<= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator>>= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator*= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator/= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator%= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator+= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator-= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator&= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator|= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator^= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator[] (T && t);

        /** \see BOOST_YAP_USER_MEMBER_CALL_OPERATOR for full semantics. */
        template <typename ...T> auto operator() (T && ... t);

#else

#define BOOST_YAP_UNARY_MEMBER_OPERATOR(op_name)                    \
        BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(op_name, ::boost::yap::expression)

        BOOST_YAP_UNARY_MEMBER_OPERATOR(unary_plus) // +
        BOOST_YAP_UNARY_MEMBER_OPERATOR(negate) // -
        BOOST_YAP_UNARY_MEMBER_OPERATOR(dereference) // *
        BOOST_YAP_UNARY_MEMBER_OPERATOR(complement) // ~
        BOOST_YAP_UNARY_MEMBER_OPERATOR(address_of) // &
        BOOST_YAP_UNARY_MEMBER_OPERATOR(logical_not) // !
        BOOST_YAP_UNARY_MEMBER_OPERATOR(pre_inc) // ++
        BOOST_YAP_UNARY_MEMBER_OPERATOR(pre_dec) // --
        BOOST_YAP_UNARY_MEMBER_OPERATOR(post_inc) // ++(int)
        BOOST_YAP_UNARY_MEMBER_OPERATOR(post_dec) // --(int)

#undef BOOST_YAP_UNARY_MEMBER_OPERATOR

        // TODO: Add test coverage for all the operators (with all three qual
        // types), for expression and terminal.  Don't forget the free
        // operators.
#define BOOST_YAP_BINARY_MEMBER_OPERATOR(op_name)                   \
        BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(op_name, ::boost::yap::expression)

        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_left) // <<
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_right) // >>
        BOOST_YAP_BINARY_MEMBER_OPERATOR(multiplies) // *
        BOOST_YAP_BINARY_MEMBER_OPERATOR(divides) // /
        BOOST_YAP_BINARY_MEMBER_OPERATOR(modulus) // %
        BOOST_YAP_BINARY_MEMBER_OPERATOR(plus) // +
        BOOST_YAP_BINARY_MEMBER_OPERATOR(minus) // -
        BOOST_YAP_BINARY_MEMBER_OPERATOR(less) // <
        BOOST_YAP_BINARY_MEMBER_OPERATOR(greater) // >
        BOOST_YAP_BINARY_MEMBER_OPERATOR(less_equal) // <=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(greater_equal) // >=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(equal_to) // ==
        BOOST_YAP_BINARY_MEMBER_OPERATOR(not_equal_to) // !=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(logical_or) // ||
        BOOST_YAP_BINARY_MEMBER_OPERATOR(logical_and) // &&
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_and) // &
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_or) // |
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_xor) // ^
        BOOST_YAP_BINARY_MEMBER_OPERATOR(comma) // ,
        BOOST_YAP_BINARY_MEMBER_OPERATOR(mem_ptr) // ->*
        BOOST_YAP_BINARY_MEMBER_OPERATOR(assign) // =
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_left_assign) // <<=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_right_assign) // >>=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(multiplies_assign) // *=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(divides_assign) // /=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(modulus_assign) // %=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(plus_assign) // +=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(minus_assign) // -=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_and_assign) // &=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_or_assign) // |=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_xor_assign) // ^=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(subscript) // []

#undef BOOST_YAP_BINARY_MEMBER_OPERATOR

        BOOST_YAP_USER_MEMBER_CALL_OPERATOR(::boost::yap::expression)

#endif // BOOST_YAP_DOXYGEN
    };

    /** Terminal expression specialization of the reference expression
        template.

        \note Due to a limitation of Doxygen, the <code>value()</code> member
        and each of the operator overloads listed here is a stand-in for three
        member functions.  For each function <code>f</code>, the listing here
        is: \code return_type f (); \endcode However, there are actually three
        functions:
        \code
        return_type f () const &;
        return_type f () &;
        return_type f () &&;
        \endcode
    */
    template <typename T>
    struct expression<expr_kind::terminal, hana::tuple<T>>
    {
        using tuple_type = hana::tuple<T>;

        static const expr_kind kind = expr_kind::terminal;

        /** Default constructor.  Does nothing. */
        expression () {}

        /** Forwards \a t into \c elements. */
        expression (T && t) :
            elements (static_cast<T &&>(t))
        {}

        /** Moves \a rhs into the only data mamber, \c elements. */
        expression (hana::tuple<T> && rhs) :
            elements (std::move(rhs))
        {}

        tuple_type elements;

        /** A convenience member function that dispatches to the free function
            <code>value()</code>. */
        decltype(auto) value () &
        { return ::boost::yap::value(*this); }

#ifndef BOOST_YAP_DOXYGEN

        decltype(auto) value () const &
        { return ::boost::yap::value(*this); }

        decltype(auto) value () &&
        { return ::boost::yap::value(std::move(*this)); }

#endif

#ifdef BOOST_YAP_DOXYGEN

        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator+ ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator- ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator* ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator~ ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator& ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator! ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator++ ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator-- ();
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator++ (int);
        /** \see BOOST_YAP_USER_UNARY_OPERATOR_MEMBER for full semantics. */
        auto operator-- (int);

        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator<< (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator>> (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator* (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator/ (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator% (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator+ (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator- (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator< (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator> (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator<= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator>= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator== (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator!= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator|| (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator&& (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator& (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator| (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator^ (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator, (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator->* (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator<<= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator>>= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator*= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator/= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator%= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator+= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator-= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator&= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator|= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator^= (T && t);
        /** \see BOOST_YAP_USER_BINARY_OPERATOR_MEMBER for full semantics. */
        template <typename T> auto operator[] (T && t);

        /** \see BOOST_YAP_USER_MEMBER_CALL_OPERATOR for full semantics. */
        template <typename ...T> auto operator() (T && ... t);

#else

#define BOOST_YAP_UNARY_MEMBER_OPERATOR(op_name)                    \
        BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(op_name, ::boost::yap::expression)

        BOOST_YAP_UNARY_MEMBER_OPERATOR(unary_plus) // +
        BOOST_YAP_UNARY_MEMBER_OPERATOR(negate) // -
        BOOST_YAP_UNARY_MEMBER_OPERATOR(dereference) // *
        BOOST_YAP_UNARY_MEMBER_OPERATOR(complement) // ~
        BOOST_YAP_UNARY_MEMBER_OPERATOR(address_of) // &
        BOOST_YAP_UNARY_MEMBER_OPERATOR(logical_not) // !
        BOOST_YAP_UNARY_MEMBER_OPERATOR(pre_inc) // ++
        BOOST_YAP_UNARY_MEMBER_OPERATOR(pre_dec) // --
        BOOST_YAP_UNARY_MEMBER_OPERATOR(post_inc) // ++(int)
        BOOST_YAP_UNARY_MEMBER_OPERATOR(post_dec) // --(int)

#undef BOOST_YAP_UNARY_MEMBER_OPERATOR

#define BOOST_YAP_BINARY_MEMBER_OPERATOR(op_name)                   \
        BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(op_name, ::boost::yap::expression)

        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_left) // <<
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_right) // >>
        BOOST_YAP_BINARY_MEMBER_OPERATOR(multiplies) // *
        BOOST_YAP_BINARY_MEMBER_OPERATOR(divides) // /
        BOOST_YAP_BINARY_MEMBER_OPERATOR(modulus) // %
        BOOST_YAP_BINARY_MEMBER_OPERATOR(plus) // +
        BOOST_YAP_BINARY_MEMBER_OPERATOR(minus) // -
        BOOST_YAP_BINARY_MEMBER_OPERATOR(less) // <
        BOOST_YAP_BINARY_MEMBER_OPERATOR(greater) // >
        BOOST_YAP_BINARY_MEMBER_OPERATOR(less_equal) // <=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(greater_equal) // >=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(equal_to) // ==
        BOOST_YAP_BINARY_MEMBER_OPERATOR(not_equal_to) // !=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(logical_or) // ||
        BOOST_YAP_BINARY_MEMBER_OPERATOR(logical_and) // &&
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_and) // &
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_or) // |
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_xor) // ^
        BOOST_YAP_BINARY_MEMBER_OPERATOR(comma) // ,
        BOOST_YAP_BINARY_MEMBER_OPERATOR(mem_ptr) // ->*
        BOOST_YAP_BINARY_MEMBER_OPERATOR(assign) // =
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_left_assign) // <<=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(shift_right_assign) // >>=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(multiplies_assign) // *=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(divides_assign) // /=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(modulus_assign) // %=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(plus_assign) // +=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(minus_assign) // -=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_and_assign) // &=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_or_assign) // |=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(bitwise_xor_assign) // ^=
        BOOST_YAP_BINARY_MEMBER_OPERATOR(subscript) // []

#undef BOOST_YAP_BINARY_MEMBER_OPERATOR

        BOOST_YAP_USER_MEMBER_CALL_OPERATOR(::boost::yap::expression)

#endif // BOOST_YAP_DOXYGEN
    };

    /** Returns <code>make_expression<boost::yap::expression, Kind>(...)</code>. */
    template <expr_kind Kind, typename ...T>
    auto make_expression (T && ... t)
    { return make_expression<expression, Kind>(static_cast<T &&>(t)...); }

    /** Returns <code>make_terminal<boost::yap::expression>(t)</code>. */
    template <typename T>
    auto make_terminal (T && t)
    { return make_terminal<expression>(static_cast<T &&>(t)); }

    /** Returns <code>as_expr<boost::yap::expression>(t)</code>. */
    template <typename T>
    decltype(auto) as_expr (T && t)
    { return as_expr<expression>(static_cast<T &&>(t)); }

} }

#endif
