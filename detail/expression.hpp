#ifndef BOOST_PROTO17_DETAIL_EXPRESSION_HPP_INCLUDED
#define BOOST_PROTO17_DETAIL_EXPRESSION_HPP_INCLUDED

#include "../expression_fwd.hpp"

#include <boost/hana/tuple.hpp>

#include <type_traits>


namespace boost::proto17 {

    namespace detail {

        template <typename T>
        struct partial_decay
        { using type = T; };

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

        template <
            typename T,
            typename U = typename detail::partial_decay<T>::type,
            bool AddRValueRef = std::is_same_v<T, U> && !std::is_const_v<U>
        >
        struct operand_value_type_phase_1;

        template <typename T, typename U>
        struct operand_value_type_phase_1<T, U, true>
        { using type = U &&; };

        template <typename T, typename U>
        struct operand_value_type_phase_1<T, U, false>
        { using type = U; };

        template <typename T>
        struct remove_cv_ref : std::remove_cv<std::remove_reference_t<T>>
        {};

        template <typename T>
        using remove_cv_ref_t = typename remove_cv_ref<T>::type;

        template <typename T>
        struct is_hana_tuple
        { static bool const value = false; };

        template <typename ...T>
        struct is_hana_tuple<hana::tuple<T...>>
        { static bool const value = true; };

        template <typename Expr, typename = std::void_t<>, typename = std::void_t<>>
        struct is_expr
        { static bool const value = false; };

        template <typename Expr>
        struct is_expr<
            Expr,
            std::void_t<decltype(Expr::kind)>,
            std::void_t<decltype(std::declval<Expr>().elements)>
        >
        {
            static bool const value =
                std::is_same<std::remove_cv_t<decltype(Expr::kind)>, expr_kind>{} &&
                is_hana_tuple<remove_cv_ref_t<decltype(std::declval<Expr>().elements)>>::value;
        };

        template <typename T>
        struct expr_ref
        { using type = expression_ref<T>; };

        template <typename Tuple>
        struct expr_ref<expression<expr_kind::expr_ref, Tuple> &>
        { using type = expression<expr_kind::expr_ref, Tuple>; };

        template <typename Tuple>
        struct expr_ref<expression<expr_kind::expr_ref, Tuple> const &>
        { using type = expression<expr_kind::expr_ref, Tuple>; };

        template <typename T>
        using expr_ref_t = typename expr_ref<T>::type;

        template <typename T>
        struct expr_ref_tuple;

        template <typename Tuple>
        struct expr_ref_tuple<expression<expr_kind::expr_ref, Tuple>>
        { using type = Tuple; };

        template <typename T>
        using expr_ref_tuple_t = typename expr_ref_tuple<T>::type;

        template <
            typename T,
            typename U = typename operand_value_type_phase_1<T>::type,
            bool RemoveRefs = std::is_rvalue_reference_v<U>,
            bool IsExpr = is_expr<remove_cv_ref_t<T>>::value,
            bool IsLRef = std::is_lvalue_reference<T>{}
        >
        struct operand_type;

        template <typename T, typename U, bool RemoveRefs>
        struct operand_type<T, U, RemoveRefs, true, false>
        { using type = remove_cv_ref_t<T>; };

        template <typename T, typename U, bool RemoveRefs>
        struct operand_type<T, U, RemoveRefs, true, true>
        { using type = expr_ref_t<T>; };

        template <typename T, typename U, bool RemoveRefs, bool IsLRef>
        struct operand_type<T, U, RemoveRefs, true, IsLRef>
        { using type = remove_cv_ref_t<T>; };

        template <typename T, typename U, bool IsLRef>
        struct operand_type<T, U, true, false, IsLRef>
        { using type = terminal<std::remove_reference_t<U>>; };

        template <typename T, typename U, bool IsLRef>
        struct operand_type<T, U, false, false, IsLRef>
        { using type = terminal<U>; };

        template <typename T>
        using operand_type_t = typename operand_type<T>::type;

        template <typename R>
        struct make_operand
        {
            template <typename U>
            auto operator() (U && u)
            { return R{static_cast<U &&>(u)}; }
        };

        template <typename Tuple>
        struct make_operand<expression<expr_kind::expr_ref, Tuple>>
        {
            auto operator() (expression<expr_kind::expr_ref, Tuple> expr)
            { return expr; }

            template <typename U>
            auto operator() (U && u)
            { return expression<expr_kind::expr_ref, Tuple>{Tuple{std::addressof(u)}}; }
        };

        template <
            expr_kind OpKind,
            typename T,
            typename U,
            template <expr_kind, class, class ...> class expr_template,
            bool TNonExprUExpr =
                !detail::is_expr<remove_cv_ref_t<T>>::value &&
                detail::is_expr<remove_cv_ref_t<U>>::value
        >
        struct free_binary_op_result
        {
            using lhs_type = detail::operand_type_t<T>;
            using rhs_type = detail::expr_ref_t<U>;
            using rhs_tuple_type = detail::expr_ref_tuple_t<rhs_type>;
            using type = expr_template<OpKind, hana::tuple<lhs_type, rhs_type>>;
        };

        template <
            expr_kind OpKind,
            typename T,
            typename U,
            template <expr_kind, class, class ...> class expr_template
        >
        struct free_binary_op_result<OpKind, T, U, expr_template, false>
        {};

        template <
            expr_kind OpKind,
            typename T,
            typename U,
            template <expr_kind, class, class ...> class expr_template
        >
        using free_binary_op_result_t = typename free_binary_op_result<OpKind, T, U, expr_template>::type;

    }

}

#endif
