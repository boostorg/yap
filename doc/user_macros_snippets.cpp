#include <boost/yap/expression.hpp>

#include <string>
#include <vector>


#define user_expr user_expr_1

/// [USER_UNARY_OPERATOR_MEMBER]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;

    // Member operator overloads for operator!().
    BOOST_YAP_USER_UNARY_OPERATOR_MEMBER(logical_not, this_type, ::user_expr)
};
/// [USER_UNARY_OPERATOR_MEMBER]

struct lazy_vector_1 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef user_expr

#define user_expr user_expr_2

/// [USER_BINARY_OPERATOR_MEMBER]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;

    // Member operator overloads for operator&&().  These will match any value
    // on the right-hand side, even another expression.  Left as-is, there is
    // no matching overload for x && y, where x is not an expression and y is.
    // BOOST_YAP_USER_FREE_BINARY_OPERATOR can help with that.
    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(logical_and, this_type, ::user_expr)
};
/// [USER_BINARY_OPERATOR_MEMBER]

struct lazy_vector_2 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef user_expr

#define user_expr user_expr_3

/// [USER_MEMBER_CALL_OPERATOR]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;

    // Member operator overloads for operator()().  These will match any
    // number of parameters.  Each one can be any type, even another
    // expression.
    BOOST_YAP_USER_MEMBER_CALL_OPERATOR(this_type, ::user_expr)
};
/// [USER_MEMBER_CALL_OPERATOR]

struct lazy_vector_3 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef user_expr

#define user_expr user_expr_4

/// [USER_FREE_BINARY_OPERATOR]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;

    // Member operator overloads for operator&&().  These will match any value
    // on the right-hand side, even another expression.
    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(logical_and, this_type, ::user_expr)
};

// Free operator overloads for operator&&().  These will match any value on
// the left-hand side, *except* an expression; the right-hand side must be an
// expression.
BOOST_YAP_USER_FREE_BINARY_OPERATOR(logical_and, ::user_expr)
/// [USER_FREE_BINARY_OPERATOR]

struct lazy_vector_4 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef user_expr

#define user_expr user_expr_5

/// [USER_EXPR_IF_ELSE]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;
};

// Defines an overload of if_else() that returns expressions instantiated from
// user_expr.
BOOST_YAP_USER_EXPR_IF_ELSE(::user_expr)
/// [USER_EXPR_IF_ELSE]

struct lazy_vector_5 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef user_expr

#define user_expr user_expr_6
#define is_vector is_vector_1

/// [USER_UDT_ANY_IF_ELSE]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;
};

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

// Defines an overload of if_else() that returns expressions instantiated from
// user_expr.
BOOST_YAP_USER_UDT_ANY_IF_ELSE(::user_expr, is_vector)
/// [USER_UDT_ANY_IF_ELSE]

struct lazy_vector_6 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef is_vector
#undef user_expr

#define user_expr user_expr_7
#define is_vector is_vector_2

/// [USER_UDT_UNARY_OPERATOR]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;
};

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

// Defines an overload of operator!() that applies to vectors and returns
// expressions instantiated from user_expr.
BOOST_YAP_USER_UDT_UNARY_OPERATOR(logical_not, ::user_expr, is_vector)
/// [USER_UDT_UNARY_OPERATOR]

struct lazy_vector_7 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef is_vector
#undef user_expr

#define user_expr user_expr_8
#define is_vector is_vector_3

/// [USER_UDT_UDT_BINARY_OPERATOR]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;
};

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

template <typename T>
struct is_string : std::false_type {};

template <>
struct is_string<std::string> : std::true_type {};

// Defines an overload of operator||() that applies to vectors on the left and
// strings on the right, and returns expressions instantiated from user_expr.
BOOST_YAP_USER_UDT_UDT_BINARY_OPERATOR(logical_or, ::user_expr, is_vector, is_string)

// Defines an overload of operator&&() that applies to strings and returns
// expressions instantiated from user_expr.
BOOST_YAP_USER_UDT_UDT_BINARY_OPERATOR(logical_and, ::user_expr, is_string, is_string)
/// [USER_UDT_UDT_BINARY_OPERATOR]

struct lazy_vector_8 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef is_vector
#undef user_expr

#define user_expr user_expr_9
#define is_vector is_vector_4

/// [USER_UDT_ANY_BINARY_OPERATOR]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;
};

template <typename T>
struct is_vector : std::false_type {};

template <typename T, typename A>
struct is_vector<std::vector<T, A>> : std::true_type {};

// Defines an overload of operator&&() that matches a string on either side,
// and any type (possibly a string) on the other.
BOOST_YAP_USER_UDT_ANY_BINARY_OPERATOR(logical_and, ::user_expr, is_string)
/// [USER_UDT_ANY_BINARY_OPERATOR]

struct lazy_vector_9 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef is_vector
#undef user_expr

#define user_expr user_expr_10

/// [USER_LITERAL_PLACEHOLDER_OPERATOR]
template <boost::yap::expr_kind Kind, typename Tuple>
struct user_expr
{
    using this_type = user_expr<Kind, Tuple>;

    static const boost::yap::expr_kind kind = Kind;

    Tuple elements;
};

namespace literals {

    // Defines a user literal operator that makes placeholders; 2_p will be a
    // 2-placeholder instantiated from the user_expr template.
    BOOST_YAP_USER_LITERAL_PLACEHOLDER_OPERATOR(user_expr)

}
/// [USER_LITERAL_PLACEHOLDER_OPERATOR]

struct lazy_vector_10 :
    user_expr<
        boost::yap::expr_kind::terminal,
        boost::hana::tuple<std::vector<double>>
    >
{};

#undef user_expr


int main ()
{
    lazy_vector_2 v1{{std::vector<double>(4, 1.0)}};
    lazy_vector_2 v2{{std::vector<double>(4, 1.0)}};
    lazy_vector_3 v3{{std::vector<double>(4, 1.0)}};
    lazy_vector_4 v4{{std::vector<double>(4, 1.0)}};
    lazy_vector_5 v5{{std::vector<double>(4, 1.0)}};
    lazy_vector_6 v6{{std::vector<double>(4, 1.0)}};
    lazy_vector_7 v7{{std::vector<double>(4, 1.0)}};
    lazy_vector_8 v8{{std::vector<double>(4, 1.0)}};
    lazy_vector_9 v9{{std::vector<double>(4, 1.0)}};
    lazy_vector_10 v10{{std::vector<double>(4, 1.0)}};

    return 0;
}
