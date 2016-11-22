#define BOOST_PROTO17_CONVERSION_OPERATOR_TEMPLATE
#include "expression.hpp"


template <typename T>
using term = boost::proto17::terminal<T>;

namespace bp17 = boost::proto17;


namespace user {

    struct number
    {
        double value;

        friend number operator+ (number lhs, number rhs)
        { return number{lhs.value + rhs.value}; }

        friend number operator* (number lhs, number rhs)
        { return number{lhs.value * rhs.value}; }
    };

    number naxpy (number a, number x, number y)
    { return number{a.value * x.value + y.value + 10.0}; }

    template <typename Expr1, typename Expr2, typename Expr3>
    decltype(auto) transform_expression (
        bp17::expression<
            bp17::expr_kind::plus,
            bp17::expression<
                bp17::expr_kind::multiplies,
                Expr1,
                Expr2
            >,
            Expr3
        > const & expr
    ) {
        return naxpy(
            evaluate(expr.left().left()),
            evaluate(expr.left().right()),
            evaluate(expr.right())
        );
    }

}

term<user::number> a{{1.0}};
term<user::number> x{{42.0}};
term<user::number> y{{3.0}};

#define HANDLE_COMMON_SUBEXPRESSION 0
#if HANDLE_COMMON_SUBEXPRESSION

user::number eval_as_proto_expr (
    decltype((a * x + y) * (a * x + y) + (a * x + y)) & expr
) {
    auto const tmp = evaluate(expr.right());
    return tmp * tmp + tmp;
}

/* LLVM produces this for eval_as_proto_expr under -O3:
   0x00000000004005e0 <+0>:     movsd  0x30(%rdi),%xmm1
   0x00000000004005e5 <+5>:     mulsd  0x38(%rdi),%xmm1
   0x00000000004005ea <+10>:    addsd  0x40(%rdi),%xmm1
   0x00000000004005ef <+15>:    addsd  0xd1(%rip),%xmm1        # 0x4006c8
   0x00000000004005f7 <+23>:    movapd %xmm1,%xmm0
   0x00000000004005fb <+27>:    mulsd  %xmm0,%xmm0
   0x00000000004005ff <+31>:    addsd  %xmm1,%xmm0
   0x0000000000400603 <+35>:    retq   
*/

user::number eval_as_proto_expr_4x (
    decltype(
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y)
    ) & expr
) {
    auto const tmp = evaluate(expr.right());
    return user::number{4} * (tmp * tmp + tmp);
}

/* LLVM produces this for eval_as_proto_expr under -O3:
*/

#else

user::number eval_as_proto_expr (
    decltype((a * x + y) * (a * x + y) + (a * x + y)) & expr
) {
    return expr;
}

/* LLVM produces this for eval_as_proto_expr under -O3:
   0x00000000004005e0 <+0>:     movsd  (%rdi),%xmm1
   0x00000000004005e4 <+4>:     movsd  0x18(%rdi),%xmm0
   0x00000000004005e9 <+9>:     mulsd  0x8(%rdi),%xmm1
   0x00000000004005ee <+14>:    addsd  0x10(%rdi),%xmm1
   0x00000000004005f3 <+19>:    movsd  0xed(%rip),%xmm2        # 0x4006e8
   0x00000000004005fb <+27>:    addsd  %xmm2,%xmm1
   0x00000000004005ff <+31>:    mulsd  0x20(%rdi),%xmm0
   0x0000000000400604 <+36>:    addsd  0x28(%rdi),%xmm0
   0x0000000000400609 <+41>:    addsd  %xmm2,%xmm0
   0x000000000040060d <+45>:    movsd  0x30(%rdi),%xmm3
   0x0000000000400612 <+50>:    mulsd  0x38(%rdi),%xmm3
   0x0000000000400617 <+55>:    addsd  0x40(%rdi),%xmm3
   0x000000000040061c <+60>:    addsd  %xmm2,%xmm3
   0x0000000000400620 <+64>:    mulsd  %xmm1,%xmm0
   0x0000000000400624 <+68>:    addsd  %xmm3,%xmm0
   0x0000000000400628 <+72>:    addsd  %xmm2,%xmm0
   0x000000000040062c <+76>:    retq   
*/

user::number eval_as_proto_expr_4x (
    decltype(
#if 0
        // TODO: Somewhere between 36 and 45 terminals, LLVM gives up on
        // inlining this function.  Document this.
        (a * x + y) * (a * x + y) + (a * x + y) +
#endif
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y)
    ) & expr
) {
    return expr;
}

/* LLVM produces this for eval_as_proto_expr_4x under -O3:
   0x00000000004005e0 <+0>:     movsd  (%rdi),%xmm1
   0x00000000004005e4 <+4>:     movsd  0x18(%rdi),%xmm0
   0x00000000004005e9 <+9>:     mulsd  0x8(%rdi),%xmm1
   0x00000000004005ee <+14>:    addsd  0x10(%rdi),%xmm1
   0x00000000004005f3 <+19>:    movsd  0x28d(%rip),%xmm2        # 0x400888
   0x00000000004005fb <+27>:    addsd  %xmm2,%xmm1
   0x00000000004005ff <+31>:    mulsd  0x20(%rdi),%xmm0
   0x0000000000400604 <+36>:    addsd  0x28(%rdi),%xmm0
   0x0000000000400609 <+41>:    addsd  %xmm2,%xmm0
   0x000000000040060d <+45>:    movsd  0x30(%rdi),%xmm3
   0x0000000000400612 <+50>:    mulsd  0x38(%rdi),%xmm3
   0x0000000000400617 <+55>:    addsd  0x40(%rdi),%xmm3
   0x000000000040061c <+60>:    addsd  %xmm2,%xmm3
   0x0000000000400620 <+64>:    mulsd  %xmm1,%xmm0
   0x0000000000400624 <+68>:    addsd  %xmm3,%xmm0
   0x0000000000400628 <+72>:    addsd  %xmm2,%xmm0
   0x000000000040062c <+76>:    retq   
*/

#endif

user::number eval_as_cpp_expr (user::number a, user::number x, user::number y)
{
    return (a * x + y) * (a * x + y) + (a * x + y);
}

/* LLVM produces this for eval_as_cpp_expr under -O3:
   0x0000000000400630 <+0>:     mulsd  %xmm1,%xmm0
   0x0000000000400634 <+4>:     addsd  %xmm2,%xmm0
   0x0000000000400638 <+8>:     movapd %xmm0,%xmm1
   0x000000000040063c <+12>:    mulsd  %xmm1,%xmm1
   0x0000000000400640 <+16>:    addsd  %xmm0,%xmm1
   0x0000000000400644 <+20>:    movapd %xmm1,%xmm0
   0x0000000000400648 <+24>:    retq   
*/


user::number eval_as_cpp_expr_4x (user::number a, user::number x, user::number y)
{
    return
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y) +
        (a * x + y) * (a * x + y) + (a * x + y);
}

/* LLVM produces this for eval_as_cpp_expr_4x under -O3:
   0x00000000004007b0 <+0>:     mulsd  %xmm1,%xmm0
   0x00000000004007b4 <+4>:     addsd  %xmm2,%xmm0
   0x00000000004007b8 <+8>:     movapd %xmm0,%xmm2
   0x00000000004007bc <+12>:    mulsd  %xmm2,%xmm2
   0x00000000004007c0 <+16>:    movapd %xmm0,%xmm1
   0x00000000004007c4 <+20>:    addsd  %xmm2,%xmm1
   0x00000000004007c8 <+24>:    addsd  %xmm2,%xmm1
   0x00000000004007cc <+28>:    addsd  %xmm0,%xmm1
   0x00000000004007d0 <+32>:    addsd  %xmm2,%xmm1
   0x00000000004007d4 <+36>:    addsd  %xmm0,%xmm1
   0x00000000004007d8 <+40>:    addsd  %xmm2,%xmm1
   0x00000000004007dc <+44>:    addsd  %xmm0,%xmm1
   0x00000000004007e0 <+48>:    movapd %xmm1,%xmm0
   0x00000000004007e4 <+52>:    retq   
*/


int main ()
{
    auto expr = (a * x + y) * (a * x + y) + (a * x + y);
    user::number result_1 = eval_as_proto_expr(expr);
    user::number result_2 = eval_as_cpp_expr(a, x, y);

    (void)result_1;
    (void)result_2;

    return 0;
}
