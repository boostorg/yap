#include "expression.hpp"

#define BOOST_PROTO17_STREAM_OPERATORS // TODO: For testing.
#include "print.hpp"

#include <boost/hana/integral_constant.hpp>
#include <boost/hana/tuple.hpp>
#include <boost/hana/type.hpp>
#include <boost/hana/size.hpp>


// TODO: Verbose debugging mode for matching.
// TODO: Proto-style "Fuzzy and Exact Matches of Terminals".

namespace boost::proto17 {

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

}


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
            term<double>,
            term<char const *>
        > unevaluated_expr = unity + "3";
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<std::string>
        > unevaluated_expr = unity + "3"s;
    }

    // arrays
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
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
            term<double>,
            term<int * &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const * &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
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
            term<double>,
            term<int * const &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        int const * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const * const &>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        int * const int_ptr = ints;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int * const>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // values
    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int const i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > unevaluated_expr = unity + std::move(i);
    }
}

void term_plus_x_this_ref_overloads()
{
    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> const unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i = 1;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = std::move(unity) + i;
    }
}

void term_plus_term ()
{
    // char const * string
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<char const *>
        > unevaluated_expr = unity + term<char const *>{"3"};
    }

    // std::string temporary
    {
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<std::string>
        > unevaluated_expr = unity + term<std::string>{"3"s};
    }

    // pointers
    {
        term<double> unity{1.0};
        int ints_[] = {1, 2};
        term<int *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int const ints_[] = {1, 2};
        term<int const *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const *>
        > unevaluated_expr = unity + ints;
    }

    {
        term<double> unity{1.0};
        int ints_[] = {1, 2};
        term<int *> ints = {ints_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int *>
        > unevaluated_expr = unity + std::move(ints);
    }

    // const pointers
    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        term<int * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int * const>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int const ints[] = {1, 2};
        term<int const * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const * const>
        > unevaluated_expr = unity + int_ptr;
    }

    {
        term<double> unity{1.0};
        int ints[] = {1, 2};
        term<int * const> int_ptr = {ints};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int * const>
        > unevaluated_expr = unity + std::move(int_ptr);
    }

    // values
    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        term<int const> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > unevaluated_expr = unity + std::move(i);
    }

    // const value terminals
    {
        term<double> unity{1.0};
        term<int> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        term<int const> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const>
        > unevaluated_expr = unity + i;
    }

    // lvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > unevaluated_expr = unity + i;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > unevaluated_expr = unity + std::move(i);
    }

    // rvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > unevaluated_expr = unity + std::move(i);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > unevaluated_expr = unity + std::move(i);
    }
}

void term_plus_expr ()
{
    // values
    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        term<int const> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int const>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int>
            >
        > unevaluated_expr = unity + expr;
    }

    // const value terminals/expressions
    {
        term<double> unity{1.0};
        term<int> const i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > const expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        term<int> i = {1};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int>
        > const expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int>
            >
        > unevaluated_expr = unity + expr;
    }

    // lvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int const &>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &>
            >
        > unevaluated_expr = unity + expr;
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > expr = unity + i;
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int const &>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    // rvalue refs
    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 1;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }
}

void placeholders ()
{
    using namespace boost::proto17::literals;

    {
        bp17::placeholder<0> p0 = 0_p;
    }

    {
        bp17::placeholder<0> p0 = 0_p;
        term<double> unity{1.0};
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<0>,
            term<double>
        > expr = p0 + unity;
    }

    {
        bp17::placeholder<0> p0 = 0_p;
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<0>,
            bp17::placeholder<1>
        > expr = p0 + 1_p;
    }
}

void const_term_expr ()
{
    {
        term<double const> unity{1.0};
        int i_ = 42;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double const>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double const>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double const>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> const unity{1.0};
        int i_ = 42;
        term<int &&> i{std::move(i_)};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        > expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int &&>
            >
        > unevaluated_expr = unity + std::move(expr);
    }

    {
        term<double> unity{1.0};
        int i_ = 42;
        term<int const &> i{i_};
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int const &>
        > const expr = unity + std::move(i);
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            bp17::expression<
                bp17::expr_kind::plus,
                term<double>,
                term<int const &>
            >
        > unevaluated_expr = unity + std::move(expr);
    }
}

void print ()
{
    term<double> unity{1.0};
    int i_ = 42;
    term<int &&> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        term<int &&>
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<double>,
            term<int &&>
        >
    > unevaluated_expr = unity + std::move(expr);

    std::cout << "================================================================================\n";
    bp17::print(std::cout, unity);
    std::cout << "================================================================================\n";
    bp17::print(std::cout, expr);
    std::cout << "================================================================================\n";
    bp17::print(std::cout, unevaluated_expr);

    struct thing {};
    term<thing> a_thing(thing{});
    std::cout << "================================================================================\n";
    bp17::print(std::cout, a_thing);

    std::cout << "\n";
    std::cout << "================================================================================\n";
    std::cout << "================================================================================\n";
    {
        using namespace boost::proto17::literals;
        std::cout << (0_p + unity);
        std::cout << (2_p + 3_p);
        std::cout << (unity + 1_p);
    }

#if defined(BOOST_PROTO17_STREAM_OPERATORS)
    std::cout << "\n";
    std::cout << "================================================================================\n";
    std::cout << "================================================================================\n";
    std::cout << unity;
    std::cout << "================================================================================\n";
    std::cout << expr;
    std::cout << "================================================================================\n";
    std::cout << unevaluated_expr;
    std::cout << "================================================================================\n";
    std::cout << a_thing;
#endif
}

void default_eval ()
{
    std::cout << "\ndefault_eva()\n";

    term<double> unity{1.0};
    int i_ = 42;
    term<int &&> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::minus,
        term<double>,
        term<int &&>
    > expr = unity - std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<double>,
        bp17::expression<
            bp17::expr_kind::minus,
            term<double>,
            term<int &&>
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        double result = unity;
        std::cout << "unity=" << result << "\n"; // 1
    }

    {
        double result = expr;
        std::cout << "expr=" << result << "\n"; // -41
    }

    {
        double result = unevaluated_expr;
        std::cout << "unevaluated_expr=" << result << "\n"; // -40
    }

    {
        double result = evaluate(unity, 5, 6, 7);
        std::cout << "evaluate(unity)=" << result << "\n"; // 1
    }

    {
        double result = evaluate(expr);
        std::cout << "evaluate(expr)=" << result << "\n"; // -41
    }

    {
        double result = evaluate(unevaluated_expr, std::string("15"));
        std::cout << "evaluate(unevaluated_expr)=" << result << "\n"; // -40
    }

    std::cout << "\n";
}

namespace test {

    struct number
    {
        explicit operator double () const { return value; }

        double value;
    };

    // User-defined binary-plus!  With weird semantics!
    template <typename A, typename B>
    inline auto eval_plus (A a, B b)
    { return number{a.value - b.value}; }

    template <typename E, typename ...T>
    constexpr auto evaluate_expression_as (
        E const & expr,
        boost::hana::basic_type<test::number>,
        T && ...t)
    {
        std::cout << "User eval!  ";
        return static_cast<test::number>(
            bp17::detail::default_eval_expr(
                expr,
                boost::hana::make_tuple(static_cast<T &&>(t)...)
            )
        );
    }

}

void user_operator_eval ()
{
    std::cout << "\nuser_operator_eval()\n";

    term<test::number> unity{{1.0}};
    double d_ = 42.0;
    term<test::number> i{{d_}};
    bp17::expression<
        bp17::expr_kind::plus,
        term<test::number>,
        term<test::number>
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<test::number>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<test::number>,
            term<test::number>
        >
    > unevaluated_expr = unity + std::move(expr);

    {
        double result = unity;
        std::cout << "unity=" << result << "\n"; // 1
    }

    {
        double result = expr;
        std::cout << "expr=" << result << "\n"; // -41
    }

    {
        double result = unevaluated_expr;
        std::cout << "unevaluated_expr=" << result << "\n"; // 42
    }

    {
        double result = (double)evaluate(unity, 5, 6, 7);
        std::cout << "evaluate(unity)=" << result << "\n"; // 1
    }

    {
        double result = (double)evaluate(expr);
        std::cout << "evaluate(expr)=" << result << "\n"; // -41
    }

    {
        double result = (double)evaluate(unevaluated_expr, std::string("15"));
        std::cout << "evaluate(unevaluated_expr)=" << result << "\n"; // 42
    }

    std::cout << "\n";
}

void user_evaluate_expression_as ()
{
    term<test::number> unity{{1.0}};
    double d_ = 42.0;
    term<test::number> i{{d_}};
    bp17::expression<
        bp17::expr_kind::plus,
        term<test::number>,
        term<test::number>
    > expr = unity + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        term<test::number>,
        bp17::expression<
            bp17::expr_kind::plus,
            term<test::number>,
            term<test::number>
        >
    > unevaluated_expr = unity + std::move(expr);

    std::cout << "\nuser_evaluate_expression_as()\n";

    {
        test::number result = unity;
        std::cout << "unity=" << result.value << "\n"; // 1
    }

    {
        test::number result = expr;
        std::cout << "expr=" << result.value << "\n"; // -41
    }

    {
        test::number result = unevaluated_expr;
        std::cout << "unevaluated_expr=" << result.value << "\n"; // 42
    }

    {
        test::number result = evaluate(unity, 5, 6, 7);
        std::cout << "evaluate(unity)=" << result.value << "\n"; // 1
    }

    {
        double result = (double)evaluate(expr);
        std::cout << "evaluate(expr)=" << result << "\n"; // -41
    }

    {
        double result = (double)evaluate(unevaluated_expr, std::string("15"));
        std::cout << "evaluate(unevaluated_expr)=" << result << "\n"; // 42
    }

    {
        test::number result = bp17::evaluate_as<test::number>(unity, 5, 6, 7);
        std::cout << "evaluate(unity)=" << result.value << "\n"; // 1
    }

    {
        test::number result = bp17::evaluate_as<test::number>(expr);
        std::cout << "evaluate(expr)=" << result.value << "\n"; // -41
    }

    {
        test::number result = bp17::evaluate_as<test::number>(unevaluated_expr, std::string("15"));
        std::cout << "evaluate(unevaluated_expr)=" << result.value << "\n"; // 42
    }

    std::cout << "\n";
}

void placeholder_eval ()
{
    std::cout << "\nplaceholder_eval()\n";

    using namespace boost::proto17::literals;

    bp17::placeholder<2> p2 = 2_p;
    int i_ = 42;
    term<int> i{std::move(i_)};
    bp17::expression<
        bp17::expr_kind::plus,
        bp17::placeholder<2>,
        term<int>
    > expr = p2 + std::move(i);
    bp17::expression<
        bp17::expr_kind::plus,
        bp17::placeholder<2>,
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::placeholder<2>,
            term<int>
        >
    > unevaluated_expr = p2 + std::move(expr);

    {
        double result = evaluate(p2, 5, 6, 7);
        std::cout << "evaluate(p2)=" << result << "\n"; // 7
    }

    {
        double result = evaluate(expr, std::string("15"), 3, 1);
        std::cout << "evaluate(expr)=" << result << "\n"; // 43
    }

    {
        double result = evaluate(unevaluated_expr, std::string("15"), 2, 3);
        std::cout << "evaluate(unevaluated_expr)=" << result << "\n"; // 48
    }

    std::cout << "\n";
}

int main ()
{
    term_plus_x();
    term_plus_x_this_ref_overloads();
    term_plus_term();
    term_plus_expr();
    placeholders();

    const_term_expr();

    print();

    default_eval();
    user_evaluate_expression_as();
    user_operator_eval();

    placeholder_eval();

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
