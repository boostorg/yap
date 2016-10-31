#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/size.hpp>


// TODO: Pretty printer.
// TODO: Verbose debugging mode for matching.
// TODO: Proto-style "Fuzzy and Exact Matches of Terminals".

namespace boost { namespace proto17 {

    enum class expr_kind {
        terminal,

        plus,
        minus,

        // etc...
    };

    template <expr_kind Kind, typename ...T>
    struct expression;

    template <typename T>
    struct terminal;

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
        struct rhs_value_type_phase_1;

        template <typename T, typename U>
        struct rhs_value_type_phase_1<T, U, true>
        { using type = U &&; };

        template <typename T, typename U>
        struct rhs_value_type_phase_1<T, U, false>
        { using type = U; };

        template <typename T>
        struct is_terminal
        { static bool const value = false; };

        template <typename T>
        struct is_terminal<terminal<T>>
        { static bool const value = true; };

        template <typename T,
                  typename U = typename rhs_value_type_phase_1<T>::type,
                  bool RemoveRefs = std::is_rvalue_reference_v<U>,
                  bool IsTerminal = is_terminal<std::decay_t<T>>::value>
        struct rhs_type;

        template <typename T, typename U, bool RemoveRefs>
        struct rhs_type<T, U, RemoveRefs, true>
        { using type = T; };

        template <typename T, typename U>
        struct rhs_type<T, U, true, false>
        { using type = terminal<std::remove_reference_t<U>>; };

        template <typename T, typename U>
        struct rhs_type<T, U, false, false>
        { using type = terminal<U>; };

    }

#if 0 // TODO
    struct callable
    {
        auto operator() (TODO) const;
    };
#endif

    // TODO: May be a Callable.
    template <typename T>
    struct terminal
    {
        T value;

        template <typename U>
        auto operator+ (U && rhs) &
        {
            using rhs_type = typename detail::rhs_type<U>::type;
            return expression<expr_kind::plus, terminal<T> &, rhs_type>{
                hana::tuple<terminal<T> &, rhs_type>(*this, rhs_type{static_cast<U &&>(rhs)})
            };
        }

#if 0
        template <typename U>
        auto operator+ (U && rhs) const &
        {
            return expression<expr_kind::plus, terminal<T> const &, hana::detail::decay(rhs)>{
                hana::tuple<terminal<T> const &, hana::detail::decay(rhs)>(*this, std::forward<U>(rhs))
            };
        }

        template <typename U>
        auto operator+ (U && rhs) &&
        {
            return expression<expr_kind::plus, terminal<T> &&, hana::detail::decay(rhs)>{
                hana::tuple<terminal<T> &&, hana::detail::decay(rhs)>(
                    static_cast<terminal<T> &&>(*this),
                    std::forward<U>(rhs)
                )
            };
        }
#endif
    };

    template <typename T>
    constexpr bool is_terminal (hana::basic_type<T>)
    { return false; }
    template <typename T>
    constexpr bool is_terminal (hana::basic_type<terminal<T>>)
    { return true; }

    template <expr_kind Kind, typename T>
    constexpr bool is_expression (hana::basic_type<T>)
    { return false; }
    template <expr_kind Kind, typename ...T>
    constexpr bool is_expression (hana::basic_type<expression<Kind, T...>>)
    { return true; }

    template <expr_kind Kind, typename ...T>
    struct expression
    {
        static const expr_kind kind = Kind;
        hana::tuple<T...> elements;
    };

    template <long long I>
    struct placeholder : hana::llong<I> {};

    namespace literals {

        template <char ...c>
        constexpr auto operator"" _p ()
        { return placeholder<hana::ic_detail::parse<sizeof...(c)>({c...})>{}; }

    }

    namespace match {

        template <typename KindMatches, typename ...T>
        struct expression
        {
            KindMatches kind_matches;
            hana::tuple<T...> elements;
        };

        // TODO: Use this to detect identical types within a match.
        template <long long I>
        struct placeholder : hana::llong<I> {};

    }

    template <typename KindMatches, typename T>
    constexpr bool is_match_expression (hana::basic_type<T>)
    { return false; }
    template <typename KindMatches, typename ...T>
    constexpr bool is_match_expression (hana::basic_type<match::expression<KindMatches, T...>>)
    { return true; }

    namespace literals {

        template <char ...c>
        constexpr auto operator"" _T ()
        { return match::placeholder<hana::ic_detail::parse<sizeof...(c)>({c...})>{}; }

    }

    namespace detail {

        inline bool matches (...)
        { return false; }

        template <typename MatchExpr, typename TreeExpr>
        bool matches (MatchExpr const & match_subtree, TreeExpr const & subtree);

        template <std::size_t I, typename MatchExpr, typename TreeExpr>
        void recursive_match_impl (MatchExpr const & match_subtree, TreeExpr const & subtree, bool & result)
        {
            if (!matches(match_subtree[hana::size_c<I>], subtree[hana::size_c<I>])) {
                result = false;
            }
            if constexpr (0 < I) {
                recursive_match_impl<I - 1>(match_subtree, subtree, result);
            }
        }

        template <typename MatchExpr, typename TreeExpr>
        bool matches (MatchExpr const & match_subtree, TreeExpr const & subtree)
        {
            static_assert(is_match_expression(hana::typeid_(match_subtree)),
                          "Attempted to use a non-tree as a match expression.");
            static_assert(is_expression(hana::typeid_(subtree)),
                          "Attempted to use find a match in a non-tree.");

            // TODO: Verbose mode.

            if (!match_subtree.kind_matches(subtree)) {
                return false;
            } else {
                auto constexpr subtree_size = hana::size(subtree.elements);
                if constexpr (hana::size(match_subtree.elements) != subtree_size) {
                    // TODO: Verbose mode.
                    return false;
                } else {
                    bool children_match = true;
                    recursive_match_impl<subtree_size - 1>(
                        match_subtree.elements,
                        subtree.elements,
                        children_match
                    );
                    return children_match;
                }
            }
        }

        template <typename Matcher, typename Callable, expr_kind Kind, typename ...T>
        auto mutate_subtrees_of (
            Matcher const & match_subtree,
            expression<Kind, T...> & tree,
            Callable && mutation
        ) {
            // TODO: Process children first.
            if (matches(match_subtree, tree)) {
                return mutation(tree);
            } else if (Kind == expr_kind::terminal) {
                return tree;
            } else {
                auto mutate_child = [&match_subtree, &mutation] (auto & t) {
                    return mutate_subtrees_of(match_subtree, t, static_cast<Callable &&>(mutation));
                };
                auto return_elements = hana::transform(tree.elements, mutate_child);
                return make_expression(Kind, std::move(return_elements));
            }
        }

    }

} }


#include <string>


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;
using namespace std::string_literals;


void term_plus_x ()
{
    // char const * string
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<char const *>
        > unevaluated_expr = unity + "3";
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<std::string>
        > unevaluated_expr = unity + "3"s;
    }

    // arrays
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int const *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int *>
        > unevaluated_expr = unity + std::move(ints);
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int * &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int const * &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int *>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int * const &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int const * const &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int * const>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // values
    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int const i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int const &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int>
        > unevaluated_expr = unity + std::move(i);
    }
}

void term_plus_term ()
{
    // char const * string
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<char const *>
        > unevaluated_expr = unity + term<char const *>{"3"};
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<std::string>
        > unevaluated_expr = unity + term<std::string>{"3"s};
    }

    // arrays
    {
        term<double> unity{1.0};
        int ints_[] = {1, 2};
        term<int *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int const ints_[] = {1, 2};
        term<int const *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int const *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int ints_[] = {1, 2};
        term<int *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int *>
        > unevaluated_expr = unity + std::move(ints);
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int * &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int const * &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int *>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int * const &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int const * const &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int * const>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // values
    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int const i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int const &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double> &,
            term<int>
        > unevaluated_expr = unity + std::move(i);
    }
}

int main ()
{
    term_plus_x();
    term_plus_term();

#if 0 // TODO
    {
        bp17::terminal<double> unity{1.0};

        auto unevaluated_expr = unity + "3";
        auto mutated_expr = mutate(unevaluated_expr, match_expr, mutation);
        auto result = bp17::eval(unevaluated_expr);
    }

    {
        bp17::terminal<double> a{1.0};
        bp17::terminal<double> x{2.0};
        bp17::terminal<int> b{3};

        auto unevaluated_expr = a * x + b;
        auto match_expr = 0_T * 0_T + 1_T;
        auto mutated_expr = mutate(unevaluated_expr, match_expr, mutation);
        auto result = bp17::eval(mutated_expr);
    }

    {
        bp17::terminal<double> a{1.0};
        bp17::terminal<double> x{2.0};
        bp17::terminal<int> b{3};

        auto match_double_2 = [] (auto && terminal) {
            if constexpr (hana::typeid_(terminal) == hana::type<double>{})
                return terminal == 2.0;
            else
                return false;
        };

        auto unevaluated_expr = a * x + b;
        auto match_expr = bp17::match<double> * match_double_2 + 0_T;
        auto mutated_expr = mutate(unevaluated_expr, match_expr, mutation);
        auto result = bp17::eval(mutated_expr);
    }
#endif
}
