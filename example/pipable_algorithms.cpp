// Copyright (C) 2016-2018 T. Zachary Laine
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/yap/yap.hpp>

#include <algorithm>
#include <vector>


enum class algorithm_t { sort, unique };

template<typename Iter>
struct simple_range
{
    Iter begin() const { return first_; }
    Iter end() const { return last_; }

    Iter first_;
    Iter last_;
};

template<typename Range>
auto call_algorithm(algorithm_t a, Range & r)
{
    simple_range<decltype(r.begin())> retval{r.begin(), r.end()};
    if (a == algorithm_t::sort) {
        std::sort(r.begin(), r.end());
    } else if (a == algorithm_t::unique) {
        retval.last_ = std::unique(r.begin(), r.end());
    }
    return retval;
}

struct algorithm_eval
{
    template<typename LExpr>
    auto operator()(
        boost::yap::expr_tag<boost::yap::expr_kind::bitwise_or>,
        LExpr && left,
        algorithm_t right)
    {
        auto const left_result =
            boost::yap::transform(std::forward<LExpr>(left), *this);
        return call_algorithm(right, left_result);
    }

    template<typename Range>
    auto operator()(
        boost::yap::expr_tag<boost::yap::expr_kind::call>,
        algorithm_t a,
        Range & r)
    {
        return call_algorithm(a, r);
    }
};

template<boost::yap::expr_kind Kind, typename Tuple>
struct algorithm_expr
{
    static boost::yap::expr_kind const kind = Kind;

    Tuple elements;

    template<typename Assignee>
    operator Assignee() const
    {
        // Exercise left for the reader: static_assert() that Assignee is some
        // sort of container type.
        auto const range = boost::yap::transform(*this, algorithm_eval{});
        return Assignee(range.begin(), range.end());
    }

    BOOST_YAP_USER_BINARY_OPERATOR_MEMBER(bitwise_or, ::algorithm_expr)
};

struct algorithm
{
    static boost::yap::expr_kind const kind = boost::yap::expr_kind::terminal;

    boost::hana::tuple<algorithm_t> elements;

    BOOST_YAP_USER_MEMBER_CALL_OPERATOR(::algorithm_expr)
};

algorithm sort{{algorithm_t::sort}};
algorithm unique{{algorithm_t::unique}};

int main()
{
{
    std::vector<int> v1 = {0, 2, 2, 7, 1, 3, 8};
    std::sort(v1.begin(), v1.end());
    auto it = std::unique(v1.begin(), v1.end());
    std::vector<int> const v2(v1.begin(), it);
    assert(v2 == std::vector<int>({0, 1, 2, 3, 7, 8}));
}
{
    std::vector<int> v1 = {0, 2, 2, 7, 1, 3, 8};
    std::vector<int> const v2 = sort(v1) | unique;
    assert(v2 == std::vector<int>({0, 1, 2, 3, 7, 8}));
}
}
