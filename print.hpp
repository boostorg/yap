#ifndef BOOST_PROTO17_PRINT_HPP_INCLUDED
#define BOOST_PROTO17_PRINT_HPP_INCLUDED

#include "expression_fwd.hpp"

#include <boost/hana/for_each.hpp>
#include <boost/type_index.hpp>
#include <iostream>


namespace boost::proto17 {

    namespace detail {

        inline std::ostream & print_kind (std::ostream & os, expr_kind kind)
        {
            switch (kind) {
            case expr_kind::plus: return os << "+";
            case expr_kind::minus: return os << "-";
            // TODO
            default: return os << "** ERROR: UNKNOWN OPERATOR! **";
            }
        }

        template <typename T>
        auto print_value (std::ostream & os, T const & x) -> decltype(os << x)
        { return os << x; }

        inline std::ostream & print_value (std::ostream & os, ...)
        { return os << "<<unprintable-value>>"; }

        template <typename T>
        std::ostream & print_type (std::ostream & os)
        {
            os << typeindex::type_id<T>().pretty_name();
            if (std::is_const_v<T>)
                os << " const";
            if (std::is_volatile_v<T>)
                os << " volatile";
            using no_cv_t = std::remove_cv_t<T>;
            if (std::is_lvalue_reference_v<T>)
                os << " &";
            if (std::is_rvalue_reference_v<T>)
                os << " &&";
            return os;
        }

        template <expr_kind Kind, typename T, typename ...Ts>
        std::ostream & print_impl (
            std::ostream & os,
            expression<Kind, T, Ts...> const & expr,
            int indent,
            char const * indent_str)
        {
            for (int i = 0; i < indent; ++i) {
                os << indent_str;
            }

            if constexpr (Kind == expr_kind::terminal) {
                using namespace hana::literals;
                static_assert(sizeof...(Ts) == 0);
                os << "term<";
                print_type<T>(os);
                os << ">[=";
                print_value(os, expr.elements[0_c]);
                os << "]\n";
            } else if constexpr (Kind == expr_kind::placeholder) {
                using namespace hana::literals;
                static_assert(sizeof...(Ts) == 0);
                os << "placeholder<" << (long long)expr.elements[0_c] << ">\n";
            } else {
                os << "expr<";
                print_kind(os, Kind);
                os << ">\n";
                hana::for_each(expr.elements, [&os, indent, indent_str](auto const & element) {
                    print_impl(os, element, indent + 1, indent_str);
                });
            }

            return os;
        }

    }

    template <expr_kind Kind, typename ...T>
    std::ostream & print (std::ostream & os, expression<Kind, T...> const & expr)
    { return detail::print_impl(os, expr, 0, "    "); }

#if defined(BOOST_PROTO17_STREAM_OPERATORS)
    template <expr_kind Kind, typename ...T>
    std::ostream & operator<< (std::ostream & os, expression<Kind, T...> const & expr)
    { return detail::print_impl(os, expr, 0, "    "); }
#endif

}

#endif
