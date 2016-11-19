#ifndef BOOST_PROTO17_DETAIL_EXPRESSION_HPP_INCLUDED
#define BOOST_PROTO17_DETAIL_EXPRESSION_HPP_INCLUDED

#include "../expression_fwd.hpp"

#include <boost/hana/tuple.hpp>

#include <type_traits>


namespace boost::proto17 {

    namespace detail {

        template <typename T>
        struct partial_decay
        {
            using type = T;
        };

        template <typename T>
        struct partial_decay<T[]> { using type = T *; };
        template <typename T, std::size_t N>
        struct partial_decay<T[N]> { using type = T *; };

        template <typename T>
        struct partial_decay<T(&)[]> { using type = T *; };
        template <typename T, std::size_t N>
        struct partial_decay<T(&)[N]> { using type = T *; };

        template <typename R, typename ...A>
        struct partial_decay<R(A...)> { using type = R(*)(A...); };
        template <typename R, typename ...A>
        struct partial_decay<R(A..., ...)> { using type = R(*)(A..., ...); };

        template <typename T,
                  typename U = typename detail::partial_decay<T>::type,
                  bool AddRValueRef = std::is_same_v<T, U> && !std::is_const_v<U>>
        struct operand_value_type_phase_1;

        template <typename T, typename U>
        struct operand_value_type_phase_1<T, U, true>
        { using type = U &&; };

        template <typename T, typename U>
        struct operand_value_type_phase_1<T, U, false>
        { using type = U; };

        template <typename ...T>
        struct is_expr
        { static bool const value = false; };

        template <expr_kind Kind, typename ...T>
        struct is_expr<expression<Kind, T...>>
        { static bool const value = true; };

        template <typename T>
        struct remove_cv_ref : std::remove_cv<std::remove_reference_t<T>>
        {};

        template <typename T>
        using remove_cv_ref_t = typename remove_cv_ref<T>::type;

        template <typename T,
                  typename U = typename operand_value_type_phase_1<T>::type,
                  bool RemoveRefs = std::is_rvalue_reference_v<U>,
                  bool IsExpr = is_expr<remove_cv_ref_t<T>>::value>
        struct operand_type;

        template <typename T, typename U, bool RemoveRefs>
        struct operand_type<T, U, RemoveRefs, true>
        { using type = remove_cv_ref_t<T>; };

        template <typename T, typename U>
        struct operand_type<T, U, true, false>
        { using type = terminal<std::remove_reference_t<U>>; };

        template <typename T, typename U>
        struct operand_type<T, U, false, false>
        { using type = terminal<U>; };

        template <typename T>
        using operand_type_t = typename operand_type<T>::type;

        template <expr_kind Kind, typename T>
        struct expression_from_tuple;

        template <expr_kind Kind, typename ...T>
        struct expression_from_tuple<Kind, hana::tuple<T...>>
        { using type = expression<Kind, T...>; };

        template <typename Tuple, typename ...T>
        constexpr auto make_call_expression (T && ...args)
        {
            return typename expression_from_tuple<expr_kind::call, Tuple>::type{
                Tuple{static_cast<T &&>(args)...}
            };
        }

    }

}

#endif
