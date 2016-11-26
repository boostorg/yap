#include "expression.hpp"

namespace bp17 = boost::proto17;

struct alternate_expr_1
{
    static const bp17::expr_kind kind = bp17::expr_kind::plus;
    boost::hana::tuple<> elements;
};

struct alternate_expr_2
{
    static const bp17::expr_kind kind = bp17::expr_kind::plus;
    boost::hana::tuple<int, double> elements;
};


struct non_expr_1
{
};

struct non_expr_2
{
    boost::hana::tuple<int, double> elements;
};

struct non_expr_3
{
    static const int kind = 0;
    boost::hana::tuple<int, double> elements;
};

struct non_expr_4
{
    int kind;
    boost::hana::tuple<int, double> elements;
};

struct non_expr_5
{
    static const bp17::expr_kind kind = bp17::expr_kind::plus;
};

struct non_expr_6
{
    static const bp17::expr_kind kind = bp17::expr_kind::plus;
    int elements;
};


void compile_is_expr ()
{
    static_assert(bp17::detail::is_hana_tuple<boost::hana::tuple<>>::value);
    static_assert(bp17::detail::is_hana_tuple<boost::hana::tuple<int, double>>::value);

    static_assert(! bp17::detail::is_hana_tuple<int>::value);

    static_assert(
        bp17::detail::is_hana_tuple<
            bp17::detail::remove_cv_ref_t<
                decltype(std::declval<bp17::terminal<double>>().elements)
            >
        >::value
    );

    static_assert(bp17::detail::is_expr<bp17::terminal<double>>::value);

    static_assert(bp17::detail::is_expr<bp17::terminal<double> const>::value);
    static_assert(bp17::detail::is_expr<bp17::terminal<double> const &>::value);
    static_assert(bp17::detail::is_expr<bp17::terminal<double> &>::value);
    static_assert(bp17::detail::is_expr<bp17::terminal<double> &&>::value);

    static_assert(bp17::detail::is_expr<bp17::placeholder<1>>::value);
    static_assert(bp17::detail::is_expr<bp17::expression<bp17::expr_kind::unary_plus, boost::hana::tuple<bp17::terminal<double>>>>::value);
    static_assert(bp17::detail::is_expr<bp17::expression<bp17::expr_kind::plus, boost::hana::tuple<bp17::terminal<double>, bp17::terminal<double>>>>::value);

    static_assert(bp17::detail::is_expr<alternate_expr_1>::value);
    static_assert(bp17::detail::is_expr<alternate_expr_2>::value);

    static_assert(! bp17::detail::is_expr<int>::value);
    static_assert(! bp17::detail::is_expr<non_expr_1>::value);
    static_assert(! bp17::detail::is_expr<non_expr_2>::value);
    static_assert(! bp17::detail::is_expr<non_expr_3>::value);
    static_assert(! bp17::detail::is_expr<non_expr_4>::value);
    static_assert(! bp17::detail::is_expr<non_expr_5>::value);
    static_assert(! bp17::detail::is_expr<non_expr_6>::value);
}
