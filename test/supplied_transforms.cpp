// Copyright (C) 2019 Paul Keir
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/yap/yap.hpp>

#include <boost/test/minimal.hpp>

namespace yap = boost::yap;

int test_main(int, char * [])
{
    // Test replacements(), which returns a transform object
    {
        using namespace boost::yap::literals;

        auto expr_in  = 1_p * 2_p;
        auto xform    = yap::replacements(6,9);
        auto expr_out = yap::transform(expr_in,xform);
        auto result   = yap::evaluate(expr_out);
        BOOST_CHECK(result == 54);
    }

    // Test evaluation(), which returns a transform object
    {
        using namespace boost::yap::literals;

        auto expr_in  = 1_p * 2_p;
        auto xform    = yap::evaluation(7,10);
        auto result   = yap::transform(expr_in,xform);
        BOOST_CHECK(result == 70);
    }

    // Test replace_placeholders(), which returns an expression 
    {
        using namespace boost::yap::literals;

        auto expr_in  = 1_p * 2_p;
        auto expr_out = yap::replace_placeholders(expr_in,8,11);
        auto result   = yap::evaluate(expr_out);
        BOOST_CHECK(result == 88);
    }

    return 0;
}

