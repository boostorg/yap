#ifndef BOOST_YAP_EXPRESSION_HPP_INCLUDED
#define BOOST_YAP_EXPRESSION_HPP_INCLUDED

#include <boost/yap/expression_fwd.hpp>
#include <boost/yap/user_macros.hpp>
#include <boost/yap/detail/expression.hpp>

#include <boost/hana/size.hpp>
#include <boost/hana/comparing.hpp>


namespace boost { namespace yap {

#ifndef BOOST_YAP_DOXYGEN

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T && ... args);

        struct eval_expression_as_fn
        {
            template <typename R, typename E, typename ...T>
            constexpr decltype(auto) operator() (E const & expr, hana::basic_type<R> rtype, T && ... args) const
            { return eval_expression_as(expr, rtype, static_cast<T &&>(args)...); }
        };

    }

    using adl_detail::eval_expression_as_fn;

    inline namespace function_objects {

        inline constexpr eval_expression_as_fn eval_expression_as{};

    }

#endif

    /** Returns the <code>char const *</code> string for the spelling of the
        C++ operator associated with \a kind. */
    inline char const * op_string (expr_kind kind)
    {
        switch (kind) {
        case expr_kind::unary_plus: return "+";
        case expr_kind::negate: return "-";
        case expr_kind::dereference: return "*";
        case expr_kind::complement: return "~";
        case expr_kind::address_of: return "&";
        case expr_kind::logical_not: return "!";
        case expr_kind::pre_inc: return "++";
        case expr_kind::pre_dec: return "--";
        case expr_kind::post_inc: return "++(int)";
        case expr_kind::post_dec: return "--(int)";

        case expr_kind::shift_left: return "<<";
        case expr_kind::shift_right: return ">>";
        case expr_kind::multiplies: return "*";
        case expr_kind::divides: return "/";
        case expr_kind::modulus: return "%";
        case expr_kind::plus: return "+";
        case expr_kind::minus: return "-";
        case expr_kind::less: return "<";
        case expr_kind::greater: return ">";
        case expr_kind::less_equal: return "<=";
        case expr_kind::greater_equal: return ">=";
        case expr_kind::equal_to: return "==";
        case expr_kind::not_equal_to: return "!=";
        case expr_kind::logical_or: return "||";
        case expr_kind::logical_and: return "&&";
        case expr_kind::bitwise_and: return "&";
        case expr_kind::bitwise_or: return "|";
        case expr_kind::bitwise_xor: return "^";
        case expr_kind::comma: return ",";
        case expr_kind::mem_ptr: return "->*";
        case expr_kind::assign: return "=";
        case expr_kind::shift_left_assign: return "<<=";
        case expr_kind::shift_right_assign: return ">>=";
        case expr_kind::multiplies_assign: return "*=";
        case expr_kind::divides_assign: return "/=";
        case expr_kind::modulus_assign: return "%=";
        case expr_kind::plus_assign: return "+=";
        case expr_kind::minus_assign: return "-=";
        case expr_kind::bitwise_and_assign: return "&=";
        case expr_kind::bitwise_or_assign: return "|=";
        case expr_kind::bitwise_xor_assign: return "^=";
        case expr_kind::subscript: return "[]";

        case expr_kind::if_else: return "?:";

        case expr_kind::call: return "()";

        default: return "** ERROR: UNKNOWN OPERATOR! **";
        }
    }

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
        using this_type = expression<Kind, Tuple>;
        using tuple_type = Tuple;

        static const expr_kind kind = Kind;

        /** Default constructor.  Does nothing. */
        expression () {}

        /** Moves \a rhs into the only data mamber, \c elements. */
        expression (tuple_type && rhs) :
            elements (std::move(rhs))
        {}

        tuple_type elements;

#if BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE || defined(BOOST_YAP_DOXYGEN)
        /** Conversion operator.  Attempts to convert \c *this to any type \a
            R with a call to the <code>eval_expression_as()</code>
            customization point.  This function is only defined if the
            configuration macro
            <code>BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE</code> has been
            defined by the user to be nonzero. */
        template <typename R>
        operator R ()
        { return eval_expression_as(*this, hana::basic_type<R>{}); }
#endif

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
        BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(op_name, this_type, ::boost::yap::expression)

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
        BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(op_name, this_type, ::boost::yap::expression)

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

        BOOST_YAP_USER_MEMBER_CALL_OPERATOR(this_type, ::boost::yap::expression)

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
        using this_type = expression<expr_kind::terminal, hana::tuple<T>>;
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

#if BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE || defined(BOOST_YAP_DOXYGEN)
        /** Conversion operator.  Attempts to convert \c *this to any type \a
            R with a call to the <code>eval_expression_as()</code>
            cutomization point.  This function is only defined if the
            configuration macro
            <code>BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE</code> has been
            defined by the user to be nonzero. */
        template <typename R>
        operator R ()
        { return eval_expression_as(*this, hana::basic_type<R>{}); }
#endif

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
        BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(op_name, this_type, ::boost::yap::expression)

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
        BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(op_name, this_type, ::boost::yap::expression)

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

        BOOST_YAP_USER_MEMBER_CALL_OPERATOR(this_type, ::boost::yap::expression)

#endif // BOOST_YAP_DOXYGEN
    };

    /** "Dereferences" a reference-expression, forwarding its referent to the
        caller. */
    template <typename Expr>
    decltype(auto) deref (Expr && expr)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "deref() is only defined for expressions."
        );

        static_assert(
            detail::remove_cv_ref_t<Expr>::kind == expr_kind::expr_ref,
            "deref() is only defined for expr_ref-kind expressions."
        );

        using namespace hana::literals;
        if constexpr (
            std::is_rvalue_reference<Expr>{} &&
            !std::is_const<std::remove_reference_t<Expr>>{}
        ) {
            return std::move(*expr.elements[0_c]);
        } else {
            return *expr.elements[0_c];
        }
    }

    /** Forwards the sole element of \a x to the caller, possibly calling
        <code>deref()</code> first if \a x is a reference expression, or
        forwards \a x to the caller unchanged.

        More formally:

        - If \a x is not an expression, \a x is forwarded to the caller.

        - Otherwise, if \a x is a reference expression, the result is
        <code>value(deref(x))</code>.

        - Otherwise, if \a x is an expression with only one value (a unary
        expression or a terminal expression), the result is the forwarded
        first element of \a x.

        - Otherwise, \a x is forwarded to the caller. */
    template <typename T>
    decltype(auto) value (T && x)
    {
        if constexpr (detail::is_expr<T>::value) {
            using namespace hana::literals;
            constexpr expr_kind kind = detail::remove_cv_ref_t<T>::kind;
            constexpr detail::expr_arity arity = detail::arity_of<kind>();
            if constexpr (kind == expr_kind::expr_ref) {
                return ::boost::yap::value(::boost::yap::deref(static_cast<T &&>(x)));
            } else if constexpr (arity == detail::expr_arity::one) {
                if constexpr (std::is_lvalue_reference<T>{}) {
                    return x.elements[0_c];
                } else {
                    return std::move(x.elements[0_c]);
                }
            } else {
                return static_cast<T &&>(x);
            }
        } else {
            return static_cast<T &&>(x);
        }
    }

    /** Forwards the <i>i</i>-th element of \a expr to the caller.  If \a expr
        is a reference expression, the result is <code>get(deref(expr),
        i)</code>.

        \note <code>get()</code> is only valid if \a Expr is an expression.
    */
    template <long long I, typename Expr>
    decltype(auto) get (Expr && expr, hana::llong<I> i)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "get() is only defined for expressions."
        );

        using namespace hana::literals;
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind == expr_kind::expr_ref) {
            return ::boost::yap::get(::boost::yap::deref(static_cast<Expr &&>(expr)), i);
        } else {
            static_assert(
                0 <= I && I < decltype(hana::size(expr.elements))::value,
                "In get(expr, I), I must be nonnegative, and less "
                "than hana::size(expr.elements)."
            );
            if constexpr (std::is_lvalue_reference<Expr>{}) {
                return expr.elements[i];
            } else {
                return std::move(expr.elements[i]);
            }
        }
    }

    /** Returns <code>get(expr, boost::hana::llong_c<I>)</code>. */
    template <long long I, typename Expr>
    decltype(auto) get_c (Expr && expr)
    { return ::boost::yap::get(static_cast<Expr &&>(expr), hana::llong_c<I>); }

    /** Returns <code>get(expr, 0_c)</code>.

        \note <code>left()</code> is only valid if \a Expr is a binary
        operator expression.
    */
    template <typename Expr>
    decltype(auto) left (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 0_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind != expr_kind::expr_ref) {
            static_assert(
                detail::arity_of<kind>() == detail::expr_arity::two,
                "left() is only defined for binary expressions."
            );
        }
    }

    /** Returns <code>get(expr, 1_c)</code>.

        \note <code>right()</code> is only valid if \a Expr is a binary
        operator expression.
    */
    template <typename Expr>
    decltype(auto) right (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 1_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind != expr_kind::expr_ref) {
            static_assert(
                detail::arity_of<kind>() == detail::expr_arity::two,
                "right() is only defined for binary expressions."
            );
        }
    }

    /** Returns <code>get(expr, 0_c)</code>.

        \note <code>cond()</code> is only valid if \a Expr is an
        <code>expr_kind::if_else</code> expression.
    */
    template <typename Expr>
    decltype(auto) cond (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 0_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind != expr_kind::expr_ref) {
            static_assert(
                kind == expr_kind::if_else,
                "cond() is only defined for if_else expressions."
            );
        }
    }

    /** Returns <code>get(expr, 1_c)</code>.

        \note <code>then()</code> is only valid if \a Expr is an
        <code>expr_kind::if_else</code> expression.
    */
    template <typename Expr>
    decltype(auto) then (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 1_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind != expr_kind::expr_ref) {
            static_assert(
                kind == expr_kind::if_else,
                "then() is only defined for if_else expressions."
            );
        }
    }

    /** Returns <code>get(expr, 2_c)</code>.

        \note <code>else_()</code> is only valid if \a Expr is an
        <code>expr_kind::if_else</code> expression.
    */
    template <typename Expr>
    decltype(auto) else_ (Expr && expr)
    {
        using namespace hana::literals;
        return ::boost::yap::get(static_cast<Expr &&>(expr), 2_c);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind != expr_kind::expr_ref) {
            static_assert(
                kind == expr_kind::if_else,
                "else_() is only defined for if_else expressions."
            );
        }
    }

    /** Returns <code>get(expr, i)</code>.

        \note <code>argument()</code> is only valid if \a Expr is an
        <code>expr_kind::call</code> expression.
    */
    template <long long I, typename Expr>
    decltype(auto) argument (Expr && expr, hana::llong<I> i)
    {
        return ::boost::yap::get(static_cast<Expr &&>(expr), i);
        constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
        if constexpr (kind != expr_kind::expr_ref) {
            static_assert(
                detail::arity_of<kind>() == detail::expr_arity::n,
                "argument() is only defined for call expressions."
            );
            static_assert(
                0 <= I && I < decltype(hana::size(expr.elements))::value,
                "In argument(expr, I), I must be nonnegative, and less "
                "than hana::size(expr.elements)."
            );
        }
    }

    /** Makes a new expression instantiated from the expression template \a
        ExprTemplate, of kind \a Kind, with the given values as its elements.

        For each parameter P:

        - If P is an expression, P is moved into the result if P is an rvalue
        and captured by reference into the result otherwise.

        - Otherwise, P is wrapped in a terminal expression.

        \note <code>make_expression()</code> is only valid if the number of
        parameters passed is appropriate for \a Kind.
    */
    template <template <expr_kind, class> class ExprTemplate, expr_kind Kind, typename ...T>
    auto make_expression (T && ... t)
    {
        constexpr detail::expr_arity arity = detail::arity_of<Kind>();
        static_assert(
            (arity == detail::expr_arity::one && sizeof...(T) == 1) ||
            (arity == detail::expr_arity::two && sizeof...(T) == 2) ||
            (arity == detail::expr_arity::three && sizeof...(T) == 3) ||
            arity == detail::expr_arity::n
        );
        using tuple_type = hana::tuple<detail::operand_type_t<ExprTemplate, T>...>;
        return ExprTemplate<Kind, tuple_type>{
            tuple_type{
                detail::make_operand<detail::operand_type_t<ExprTemplate, T>>{}(static_cast<T &&>(t))...
            }
        };
    }

    /** Returns <code>make_expression<boost::yap::expression, Kind>(...)</code>. */
    template <expr_kind Kind, typename ...T>
    auto make_expression (T && ... t)
    { return make_expression<expression, Kind>(static_cast<T &&>(t)...); }

    /** Makes a new terminal expression instantiated from the expression
        template \a ExprTemplate, with the given value as its sole element.

        \note <code>make_terminal()</code> is only valid if \a T is \b not an
        expression.
    */
    template <template <expr_kind, class> class ExprTemplate, typename T>
    auto make_terminal (T && t)
    {
        static_assert(
            !detail::is_expr<T>::value,
            "make_terminal() is only defined for non expressions."
        );
        using result_type = detail::operand_type_t<ExprTemplate, T>;
        using tuple_type = decltype(std::declval<result_type>().elements);
        return result_type{tuple_type{static_cast<T &&>(t)}};
    }

    /** Returns <code>make_terminal<boost::yap::expression>(t)</code>. */
    template <typename T>
    auto make_terminal (T && t)
    { return make_terminal<expression>(static_cast<T &&>(t)); }

    /** Returns an expression formed from \a t as follows:

        - If \a t is an expression, \a t is forwarded to the caller.

        - Otherwise, \a t is wrapped in a terminal expression.
    */
    template <template <expr_kind, class> class ExprTemplate, typename T>
    decltype(auto) as_expr (T && t)
    {
        if constexpr (detail::is_expr<T>::value) {
            return static_cast<T &&>(t);
        } else {
            return make_terminal<ExprTemplate>(static_cast<T &&>(t));
        }
    }

    /** Returns <code>as_expr<boost::yap::expression>(t)</code>. */
    template <typename T>
    decltype(auto) as_expr (T && t)
    { return as_expr<expression>(static_cast<T &&>(t)); }

    /** A callable type that evaluates its contained expression when called.

        \see <code>make_expression_function()</code>
    */
    template <typename Expr>
    struct expression_function
    {
        template <typename ...U>
        decltype(auto) operator() (U && ... u)
        { return ::boost::yap::evaluate(expr, static_cast<U &&>(u)...); }

        Expr expr;
    };

    /** Returns a callable object that \a expr has been moved into.  This is
        useful for using expressions as function objects.

        \note <code>make_expression_function()</code> is only valid if \a
        Expr is an expression.
    */
    template <typename Expr>
    auto make_expression_function (Expr && expr)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "make_expression_function() is only defined for expressions."
        );
        return expression_function<Expr>{std::move(expr)};
    }

} }

#include <boost/yap/detail/default_eval.hpp>

namespace boost { namespace yap {

    /** Evaluates \a expr, substituting the subsequent parameters (if any)
        into \a expr's placeholder terminals.

        All customization points for the evaluation of expressions \b except
        <code>eval_expression_as()</code> are used to evaluate the \a expr.
        If you've overridden any, that will be reflected in the result.

        \note <code>evaluate()</code> is only valid if \a Expr is an
        expression, and <code>max_p <= sizeof...(T)</code>, where
        <code>max_p</code> is the maximum placeholder index in \a expr.
    */
    template <typename Expr, typename ...T>
    decltype(auto) evaluate (Expr && expr, T && ... t)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "evaluate() is only defined for expressions."
        );
        return detail::default_eval_expr(static_cast<Expr &&>(expr), static_cast<T &&>(t)...);
    }

    /** Evaluates \a expr, substituting the subsequent parameters (if any)
        into \a expr's placeholder terminals.  Evaluation is performed via the
        <code>eval_expression_as()</code> customization point.

        Prefer this function to <code>evaluate()</code> when you want
        evaluation to differ based on the result type.

        All customization points for the evaluation of expressions are used to
        evaluate the \a expr.  If you've overridden any, that will be
        reflected in the result.

        \note <code>evaluate()</code> is only valid if \a Expr is an
        expression, and <code>max_p <= sizeof...(T)</code>, where
        <code>max_p</code> is the maximum placeholder index in \a expr.
    */
    template <typename R, typename Expr, typename ...T>
    decltype(auto) evaluate_as (Expr && expr, T && ... t)
    {
        static_assert(
            detail::is_expr<Expr>::value,
            "evaluate_as() is only defined for expressions."
        );
        return eval_expression_as(static_cast<Expr &&>(expr), hana::basic_type<R>{}, static_cast<T &&>(t)...);
    }

    /** Returns the result of transforming (all or part of) \a expr using
        whatever overloads of <code>Transform::operator()</code> that match \a
        expr.

        \note Transformations can do anything: they may have side effects;
        they may mutate values; they may mutate types; and they may do any
        combination of these.
    */
    template <typename Expr, typename Transform>
    decltype(auto) transform (Expr && expr, Transform && transform)
    {
        if constexpr (detail::is_expr<Expr>::value) {
            constexpr expr_kind kind = detail::remove_cv_ref_t<Expr>::kind;
            return detail::default_transform_expression_tag<Expr, Transform, detail::arity_of<kind>()>{}(
                static_cast<Expr &&>(expr),
                static_cast<Transform &&>(transform)
            );
        } else {
            return static_cast<Expr &&>(expr);
        }
    }

    namespace adl_detail {

        template <typename R, typename E, typename ...T>
        constexpr decltype(auto) eval_expression_as (E const & expr, hana::basic_type<R>, T && ... args)
        { return static_cast<R>(detail::default_eval_expr(expr, static_cast<T &&>(args)...)); }

    }

} }

#endif
