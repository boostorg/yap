//[ future_group
#include <boost/yap/algorithm.hpp>

#include <boost/hana/concat.hpp>


// A custom expression template for future groups.  It supports operators ||
// and &&.
template <boost::yap::expr_kind Kind, typename Tuple>
struct future_expr
{
    using this_type = future_expr<Kind, Tuple>;

    static boost::yap::expr_kind const kind = Kind;

    future_expr (Tuple && tuple) :
        elements (std::forward<Tuple &&>(tuple))
    {}

    Tuple elements;

    // Returns the transformed/flattenen expression.
    auto get () const;

    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(logical_or, this_type, ::future_expr)
    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(logical_and, this_type, ::future_expr)
};

// A special-cased future terminal that matches the semantics from the
// original Proto example.
template <typename T>
struct future :
    future_expr<boost::yap::expr_kind::terminal, boost::hana::tuple<T>>
{
    future (T const & t = T()) :
        future_expr<boost::yap::expr_kind::terminal, boost::hana::tuple<T>> (boost::hana::tuple<T>{t})
    {}

    T get () const
    { return boost::yap::value(*this); }
};

template <typename T>
using remove_cv_ref_t = std::remove_cv_t<std::remove_reference_t<T>>;

// A transform that flattens future expressions into a tuple.
struct future_transform
{
    // Transform a terminal into its contained tuple.
    template <typename T>
    auto operator() (
        future_expr<
            boost::yap::expr_kind::terminal,
            boost::hana::tuple<T>
        > const & term
    ) {
        return term.elements;
    }

    // Transform left || right -> transform(left).
    template <typename T, typename U>
    auto operator() (
        future_expr<
            boost::yap::expr_kind::logical_or,
            boost::hana::tuple<T, U>
        > const & or_expr
    ) {
        // Assertion that left and right are compatible types.  We use value()
        // here on the results of left() and right() because that makes the
        // resulting expressions comparable if one is an expr_ref and the
        // other is not.
        static_assert(
            std::is_same<
                decltype(boost::yap::value(boost::yap::left(or_expr))),
                decltype(boost::yap::value(boost::yap::right(or_expr)))
            >{}
        );
        // Recursively transform the left side, and return the result.
        // Without the recursion, we might return a terminal expression here
        // insead of a tuple.
        return boost::yap::transform(boost::yap::left(or_expr), *this);
    }

    // Transform left && right -> concat(transform(left), transform(right)).
    template <typename T, typename U>
    auto operator() (
        future_expr<
            boost::yap::expr_kind::logical_and,
            boost::hana::tuple<T, U>
        > const & and_expr
    ) {
        // Recursively transform each side, then combine the resulting tuples
        // into a single tuple result.
        return boost::hana::concat(
            boost::yap::transform(boost::yap::left(and_expr), *this),
            boost::yap::transform(boost::yap::right(and_expr), *this)
        );
    }
};


template <boost::yap::expr_kind Kind, typename Tuple>
auto future_expr<Kind, Tuple>::get () const
{ return boost::yap::transform(*this, future_transform{}); }


// TEST CASES
struct A {};
struct B {};
struct C {};

// Called "vector" just so the code in main() will match the original Proto
// example.
template <typename ...T>
using vector = boost::hana::tuple<T...>;

int main()
{
    future<A> a;
    future<B> b;
    future<C> c;
    future<vector<A,B> > ab;

    // Verify that various future groups have the
    // correct return types.
    A                       t0 = a.get();
    vector<A, B, C>         t1 = (a && b && c).get();
    vector<A, C>            t2 = ((a || a) && c).get();
    vector<A, B, C>         t3 = ((a && b || a && b) && c).get();
    vector<vector<A, B>, C> t4 = ((ab || ab) && c).get();

    return 0;
}
//]
