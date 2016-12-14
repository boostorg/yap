#include <boost/yap/expression.hpp>

#include <boost/assign/list_of.hpp>

#include <map>
#include <iostream>


template <typename Key, typename Value, typename Allocator>
struct map_list_of_transform
{
    template <typename Fn, typename Key2, typename Value2>
    auto operator() (boost::yap::call_tag, Fn const & fn, Key2 && key, Value2 && value)
    {
        boost::yap::transform(fn, *this);
        map.try_emplace(
            Key{std::forward<Key2 &&>(key)},
            Value{std::forward<Value2 &&>(value)}
        );
        return 0;
    }

    std::map<Key, Value, Allocator> map;
};


template <boost::yap::expr_kind Kind, typename Tuple>
struct map_list_of_expr
{
    using this_type = map_list_of_expr<Kind, Tuple>;

    static boost::yap::expr_kind const kind = Kind;

    Tuple elements;

    template <typename Key, typename Value, typename Allocator>
    operator std::map<Key, Value, Allocator> () const
    {
        map_list_of_transform<Key, Value, Allocator> transform;
        boost::yap::transform(*this, transform);
        return transform.map;
    }

    BOOST_YAP_USER_MEMBER_CALL_OPERATOR(this_type, ::map_list_of_expr)
};

struct map_list_of_tag {};

auto map_list_of = boost::yap::make_terminal<map_list_of_expr>(map_list_of_tag{});


std::map<std::string, int> make_map_with_boost_yap ()
{
    return map_list_of
        ("<", 1)
        ("<=",2)
        (">", 3)
        (">=",4)
        ("=", 5)
        ("<>",6)
        ;
}

/* LLVM produces this for make_map_with_boost_yap under -O3:
   0x00000000004012e0 <+0>:	push   %rbx
   0x00000000004012e1 <+1>:	sub    $0x70,%rsp
   0x00000000004012e5 <+5>:	mov    %rdi,%rbx
   0x00000000004012e8 <+8>:	mov    $0x405f60,%eax
   0x00000000004012ed <+13>:	movq   %rax,%xmm0
   0x00000000004012f2 <+18>:	mov    $0x608251,%eax
   0x00000000004012f7 <+23>:	movq   %rax,%xmm1
   0x00000000004012fc <+28>:	punpcklqdq %xmm0,%xmm1
   0x0000000000401300 <+32>:	movdqa %xmm1,(%rsp)
   0x0000000000401305 <+37>:	movl   $0x1,0x10(%rsp)
   0x000000000040130d <+45>:	movq   $0x405f62,0x18(%rsp)
   0x0000000000401316 <+54>:	movl   $0x2,0x20(%rsp)
   0x000000000040131e <+62>:	movq   $0x405f69,0x28(%rsp)
   0x0000000000401327 <+71>:	movl   $0x3,0x30(%rsp)
   0x000000000040132f <+79>:	movq   $0x405f65,0x38(%rsp)
   0x0000000000401338 <+88>:	movl   $0x4,0x40(%rsp)
   0x0000000000401340 <+96>:	movq   $0x405f63,0x48(%rsp)
   0x0000000000401349 <+105>:	movl   $0x5,0x50(%rsp)
   0x0000000000401351 <+113>:	movq   $0x405f68,0x58(%rsp)
   0x000000000040135a <+122>:	movl   $0x6,0x60(%rsp)
   0x0000000000401362 <+130>:	mov    %rsp,%rsi
   0x0000000000401365 <+133>:	callq  0x4029f0 <_ZNK16map_list_of_exprILN5boost3yap9expr_kindE46ENS0_4hana5tupleIJS_ILS2_46ENS4_IJS_ILS2_46ENS4_IJS_ILS2_46ENS4_IJS_ILS2_46ENS4_IJS_ILS2_46ENS4_IJS_ILS2_0ENS4_IJPS_ILS2_1ENS4_IJ15map_list_of_tagEEEEEEEES_ILS2_1ENS4_IJPKcEEEES_ILS2_1ENS4_IJiEEEEEEEESE_SG_EEEESE_SG_EEEESE_SG_EEEESE_SG_EEEESE_SG_EEEEcvNSt3__13mapIT_T0_T1_NST_9allocatorINST_4pairIKSV_SW_EEEEEEINST_12basic_stringIcNST_11char_traitsIcEENSY_IcEEEEiNST_4lessIS19_EEEEv>
   0x000000000040136a <+138>:	mov    %rbx,%rax
   0x000000000040136d <+141>:	add    $0x70,%rsp
   0x0000000000401371 <+145>:	pop    %rbx
   0x0000000000401372 <+146>:	retq   
*/


std::map<std::string, int> make_map_with_boost_assign ()
{
    return boost::assign::map_list_of
        ("<", 1)
        ("<=",2)
        (">", 3)
        (">=",4)
        ("=", 5)
        ("<>",6)
        ;
}

/* LLVM produces this for make_map_with_boost_assign under -O3:
   0x0000000000401380 <+0>:	push   %r15
   0x0000000000401382 <+2>:	push   %r14
   0x0000000000401384 <+4>:	push   %r13
   0x0000000000401386 <+6>:	push   %r12
   0x0000000000401388 <+8>:	push   %rbx
   0x0000000000401389 <+9>:	sub    $0x40,%rsp
   0x000000000040138d <+13>:	mov    %rdi,%r14
   0x0000000000401390 <+16>:	movl   $0x1,0x3c(%rsp)
   0x0000000000401398 <+24>:	lea    0x8(%rsp),%rdi
   0x000000000040139d <+29>:	lea    0x3c(%rsp),%rdx
   0x00000000004013a2 <+34>:	mov    $0x405f60,%esi
   0x00000000004013a7 <+39>:	callq  0x402ae0 <boost::assign::map_list_of<char [2], int>(char const (&) [2], int const&)>
   0x00000000004013ac <+44>:	mov    0x10(%rsp),%rax
   0x00000000004013b1 <+49>:	mov    0x18(%rsp),%rcx
   0x00000000004013b6 <+54>:	mov    %rcx,%rdx
   0x00000000004013b9 <+57>:	sub    %rax,%rdx
   0x00000000004013bc <+60>:	shl    $0x5,%rdx
   0x00000000004013c0 <+64>:	dec    %rdx
   0x00000000004013c3 <+67>:	xor    %edi,%edi
   0x00000000004013c5 <+69>:	cmp    %rax,%rcx
   0x00000000004013c8 <+72>:	cmovne %rdx,%rdi
   0x00000000004013cc <+76>:	mov    0x28(%rsp),%rsi
   0x00000000004013d1 <+81>:	mov    0x30(%rsp),%rdx
   0x00000000004013d6 <+86>:	lea    (%rdx,%rsi,1),%rbx
   0x00000000004013da <+90>:	cmp    %rbx,%rdi
   0x00000000004013dd <+93>:	jne    0x4013fd <make_map_with_boost_assign()+125>
   0x00000000004013df <+95>:	lea    0x8(%rsp),%rdi
   0x00000000004013e4 <+100>:	callq  0x404f30 <std::__1::deque<std::__1::pair<char const*, int>, std::__1::allocator<std::__1::pair<char const*, int> > >::__add_back_capacity()>
   0x00000000004013e9 <+105>:	mov    0x30(%rsp),%rdx
   0x00000000004013ee <+110>:	mov    0x28(%rsp),%rsi
   0x00000000004013f3 <+115>:	mov    0x10(%rsp),%rax
   0x00000000004013f8 <+120>:	mov    0x18(%rsp),%rcx
   0x00000000004013fd <+125>:	cmp    %rax,%rcx
   0x0000000000401400 <+128>:	je     0x401419 <make_map_with_boost_assign()+153>
   0x0000000000401402 <+130>:	add    %rsi,%rdx
   0x0000000000401405 <+133>:	mov    %rdx,%rsi
   0x0000000000401408 <+136>:	shr    $0x8,%rsi
   0x000000000040140c <+140>:	movzbl %dl,%ecx
   0x000000000040140f <+143>:	shl    $0x4,%rcx
   0x0000000000401413 <+147>:	add    (%rax,%rsi,8),%rcx
   0x0000000000401417 <+151>:	jmp    0x40141b <make_map_with_boost_assign()+155>
   0x0000000000401419 <+153>:	xor    %ecx,%ecx
   0x000000000040141b <+155>:	movq   $0x405f62,(%rcx)
   0x0000000000401422 <+162>:	movl   $0x2,0x8(%rcx)
   0x0000000000401429 <+169>:	mov    0x30(%rsp),%rdi
   0x000000000040142e <+174>:	lea    0x1(%rdi),%rcx
   0x0000000000401432 <+178>:	mov    %rcx,0x30(%rsp)
   0x0000000000401437 <+183>:	mov    0x10(%rsp),%rax
   0x000000000040143c <+188>:	mov    0x18(%rsp),%rdx
   0x0000000000401441 <+193>:	mov    %rdx,%rsi
   0x0000000000401444 <+196>:	sub    %rax,%rsi
   0x0000000000401447 <+199>:	shl    $0x5,%rsi
   0x000000000040144b <+203>:	dec    %rsi
   0x000000000040144e <+206>:	xor    %ebx,%ebx
   0x0000000000401450 <+208>:	cmp    %rax,%rdx
   0x0000000000401453 <+211>:	cmovne %rsi,%rbx
   0x0000000000401457 <+215>:	mov    0x28(%rsp),%rsi
   0x000000000040145c <+220>:	lea    0x1(%rsi,%rdi,1),%rdi
   0x0000000000401461 <+225>:	cmp    %rdi,%rbx
   0x0000000000401464 <+228>:	jne    0x401484 <make_map_with_boost_assign()+260>
   0x0000000000401466 <+230>:	lea    0x8(%rsp),%rdi
   0x000000000040146b <+235>:	callq  0x404f30 <std::__1::deque<std::__1::pair<char const*, int>, std::__1::allocator<std::__1::pair<char const*, int> > >::__add_back_capacity()>
   0x0000000000401470 <+240>:	mov    0x30(%rsp),%rcx
   0x0000000000401475 <+245>:	mov    0x28(%rsp),%rsi
   0x000000000040147a <+250>:	mov    0x10(%rsp),%rax
   0x000000000040147f <+255>:	mov    0x18(%rsp),%rdx
   0x0000000000401484 <+260>:	cmp    %rax,%rdx
   0x0000000000401487 <+263>:	je     0x4014a0 <make_map_with_boost_assign()+288>
   0x0000000000401489 <+265>:	add    %rsi,%rcx
   0x000000000040148c <+268>:	mov    %rcx,%rdx
   0x000000000040148f <+271>:	shr    $0x8,%rdx
   0x0000000000401493 <+275>:	movzbl %cl,%ecx
   0x0000000000401496 <+278>:	shl    $0x4,%rcx
   0x000000000040149a <+282>:	add    (%rax,%rdx,8),%rcx
   0x000000000040149e <+286>:	jmp    0x4014a2 <make_map_with_boost_assign()+290>
   0x00000000004014a0 <+288>:	xor    %ecx,%ecx
   0x00000000004014a2 <+290>:	movq   $0x405f69,(%rcx)
   0x00000000004014a9 <+297>:	movl   $0x3,0x8(%rcx)
   0x00000000004014b0 <+304>:	mov    0x30(%rsp),%rdi
   0x00000000004014b5 <+309>:	lea    0x1(%rdi),%rcx
   0x00000000004014b9 <+313>:	mov    %rcx,0x30(%rsp)
   0x00000000004014be <+318>:	mov    0x10(%rsp),%rax
   0x00000000004014c3 <+323>:	mov    0x18(%rsp),%rdx
   0x00000000004014c8 <+328>:	mov    %rdx,%rsi
   0x00000000004014cb <+331>:	sub    %rax,%rsi
   0x00000000004014ce <+334>:	shl    $0x5,%rsi
   0x00000000004014d2 <+338>:	dec    %rsi
   0x00000000004014d5 <+341>:	xor    %ebx,%ebx
   0x00000000004014d7 <+343>:	cmp    %rax,%rdx
   0x00000000004014da <+346>:	cmovne %rsi,%rbx
   0x00000000004014de <+350>:	mov    0x28(%rsp),%rsi
   0x00000000004014e3 <+355>:	lea    0x1(%rsi,%rdi,1),%rdi
   0x00000000004014e8 <+360>:	cmp    %rdi,%rbx
   0x00000000004014eb <+363>:	jne    0x40150b <make_map_with_boost_assign()+395>
   0x00000000004014ed <+365>:	lea    0x8(%rsp),%rdi
   0x00000000004014f2 <+370>:	callq  0x404f30 <std::__1::deque<std::__1::pair<char const*, int>, std::__1::allocator<std::__1::pair<char const*, int> > >::__add_back_capacity()>
   0x00000000004014f7 <+375>:	mov    0x30(%rsp),%rcx
   0x00000000004014fc <+380>:	mov    0x28(%rsp),%rsi
   0x0000000000401501 <+385>:	mov    0x10(%rsp),%rax
   0x0000000000401506 <+390>:	mov    0x18(%rsp),%rdx
   0x000000000040150b <+395>:	cmp    %rax,%rdx
   0x000000000040150e <+398>:	je     0x401527 <make_map_with_boost_assign()+423>
   0x0000000000401510 <+400>:	add    %rsi,%rcx
   0x0000000000401513 <+403>:	mov    %rcx,%rdx
   0x0000000000401516 <+406>:	shr    $0x8,%rdx
   0x000000000040151a <+410>:	movzbl %cl,%ecx
   0x000000000040151d <+413>:	shl    $0x4,%rcx
   0x0000000000401521 <+417>:	add    (%rax,%rdx,8),%rcx
   0x0000000000401525 <+421>:	jmp    0x401529 <make_map_with_boost_assign()+425>
   0x0000000000401527 <+423>:	xor    %ecx,%ecx
   0x0000000000401529 <+425>:	movq   $0x405f65,(%rcx)
   0x0000000000401530 <+432>:	movl   $0x4,0x8(%rcx)
   0x0000000000401537 <+439>:	mov    0x30(%rsp),%rdi
   0x000000000040153c <+444>:	lea    0x1(%rdi),%rcx
   0x0000000000401540 <+448>:	mov    %rcx,0x30(%rsp)
   0x0000000000401545 <+453>:	mov    0x10(%rsp),%rax
   0x000000000040154a <+458>:	mov    0x18(%rsp),%rdx
   0x000000000040154f <+463>:	mov    %rdx,%rsi
   0x0000000000401552 <+466>:	sub    %rax,%rsi
   0x0000000000401555 <+469>:	shl    $0x5,%rsi
   0x0000000000401559 <+473>:	dec    %rsi
   0x000000000040155c <+476>:	xor    %ebx,%ebx
   0x000000000040155e <+478>:	cmp    %rax,%rdx
   0x0000000000401561 <+481>:	cmovne %rsi,%rbx
   0x0000000000401565 <+485>:	mov    0x28(%rsp),%rsi
   0x000000000040156a <+490>:	lea    0x1(%rsi,%rdi,1),%rdi
   0x000000000040156f <+495>:	cmp    %rdi,%rbx
   0x0000000000401572 <+498>:	jne    0x401592 <make_map_with_boost_assign()+530>
   0x0000000000401574 <+500>:	lea    0x8(%rsp),%rdi
   0x0000000000401579 <+505>:	callq  0x404f30 <std::__1::deque<std::__1::pair<char const*, int>, std::__1::allocator<std::__1::pair<char const*, int> > >::__add_back_capacity()>
   0x000000000040157e <+510>:	mov    0x30(%rsp),%rcx
   0x0000000000401583 <+515>:	mov    0x28(%rsp),%rsi
   0x0000000000401588 <+520>:	mov    0x10(%rsp),%rax
   0x000000000040158d <+525>:	mov    0x18(%rsp),%rdx
   0x0000000000401592 <+530>:	cmp    %rax,%rdx
   0x0000000000401595 <+533>:	je     0x4015ae <make_map_with_boost_assign()+558>
   0x0000000000401597 <+535>:	add    %rsi,%rcx
   0x000000000040159a <+538>:	mov    %rcx,%rdx
   0x000000000040159d <+541>:	shr    $0x8,%rdx
   0x00000000004015a1 <+545>:	movzbl %cl,%ecx
   0x00000000004015a4 <+548>:	shl    $0x4,%rcx
   0x00000000004015a8 <+552>:	add    (%rax,%rdx,8),%rcx
   0x00000000004015ac <+556>:	jmp    0x4015b0 <make_map_with_boost_assign()+560>
   0x00000000004015ae <+558>:	xor    %ecx,%ecx
   0x00000000004015b0 <+560>:	movq   $0x405f63,(%rcx)
   0x00000000004015b7 <+567>:	movl   $0x5,0x8(%rcx)
   0x00000000004015be <+574>:	mov    0x30(%rsp),%rdi
   0x00000000004015c3 <+579>:	lea    0x1(%rdi),%rcx
   0x00000000004015c7 <+583>:	mov    %rcx,0x30(%rsp)
   0x00000000004015cc <+588>:	mov    0x10(%rsp),%rax
   0x00000000004015d1 <+593>:	mov    0x18(%rsp),%rdx
   0x00000000004015d6 <+598>:	mov    %rdx,%rsi
   0x00000000004015d9 <+601>:	sub    %rax,%rsi
   0x00000000004015dc <+604>:	shl    $0x5,%rsi
   0x00000000004015e0 <+608>:	dec    %rsi
   0x00000000004015e3 <+611>:	xor    %ebx,%ebx
   0x00000000004015e5 <+613>:	cmp    %rax,%rdx
   0x00000000004015e8 <+616>:	cmovne %rsi,%rbx
   0x00000000004015ec <+620>:	mov    0x28(%rsp),%rsi
   0x00000000004015f1 <+625>:	lea    0x1(%rsi,%rdi,1),%rdi
   0x00000000004015f6 <+630>:	cmp    %rdi,%rbx
   0x00000000004015f9 <+633>:	jne    0x401619 <make_map_with_boost_assign()+665>
   0x00000000004015fb <+635>:	lea    0x8(%rsp),%rdi
   0x0000000000401600 <+640>:	callq  0x404f30 <std::__1::deque<std::__1::pair<char const*, int>, std::__1::allocator<std::__1::pair<char const*, int> > >::__add_back_capacity()>
   0x0000000000401605 <+645>:	mov    0x30(%rsp),%rcx
   0x000000000040160a <+650>:	mov    0x28(%rsp),%rsi
   0x000000000040160f <+655>:	mov    0x10(%rsp),%rax
   0x0000000000401614 <+660>:	mov    0x18(%rsp),%rdx
   0x0000000000401619 <+665>:	cmp    %rax,%rdx
   0x000000000040161c <+668>:	je     0x401635 <make_map_with_boost_assign()+693>
   0x000000000040161e <+670>:	add    %rsi,%rcx
   0x0000000000401621 <+673>:	mov    %rcx,%rdx
   0x0000000000401624 <+676>:	shr    $0x8,%rdx
   0x0000000000401628 <+680>:	movzbl %cl,%ecx
   0x000000000040162b <+683>:	shl    $0x4,%rcx
   0x000000000040162f <+687>:	add    (%rax,%rdx,8),%rcx
   0x0000000000401633 <+691>:	jmp    0x401637 <make_map_with_boost_assign()+695>
   0x0000000000401635 <+693>:	xor    %ecx,%ecx
   0x0000000000401637 <+695>:	movq   $0x405f68,(%rcx)
   0x000000000040163e <+702>:	movl   $0x6,0x8(%rcx)
   0x0000000000401645 <+709>:	mov    0x30(%rsp),%rcx
   0x000000000040164a <+714>:	inc    %rcx
   0x000000000040164d <+717>:	mov    %rcx,0x30(%rsp)
   0x0000000000401652 <+722>:	mov    0x10(%rsp),%rax
   0x0000000000401657 <+727>:	mov    0x28(%rsp),%rdx
   0x000000000040165c <+732>:	mov    %rdx,%rsi
   0x000000000040165f <+735>:	shr    $0x8,%rsi
   0x0000000000401663 <+739>:	lea    (%rax,%rsi,8),%r12
   0x0000000000401667 <+743>:	cmp    %rax,0x18(%rsp)
   0x000000000040166c <+748>:	je     0x401691 <make_map_with_boost_assign()+785>
   0x000000000040166e <+750>:	movzbl %dl,%ebx
   0x0000000000401671 <+753>:	shl    $0x4,%rbx
   0x0000000000401675 <+757>:	add    (%r12),%rbx
   0x0000000000401679 <+761>:	add    %rdx,%rcx
   0x000000000040167c <+764>:	mov    %rcx,%rdx
   0x000000000040167f <+767>:	shr    $0x8,%rdx
   0x0000000000401683 <+771>:	movzbl %cl,%r13d
   0x0000000000401687 <+775>:	shl    $0x4,%r13
   0x000000000040168b <+779>:	add    (%rax,%rdx,8),%r13
   0x000000000040168f <+783>:	jmp    0x401696 <make_map_with_boost_assign()+790>
   0x0000000000401691 <+785>:	xor    %ebx,%ebx
   0x0000000000401693 <+787>:	xor    %r13d,%r13d
   0x0000000000401696 <+790>:	lea    0x8(%r14),%r15
   0x000000000040169a <+794>:	xorps  %xmm0,%xmm0
   0x000000000040169d <+797>:	movups %xmm0,0x8(%r14)
   0x00000000004016a2 <+802>:	mov    %r15,(%r14)
   0x00000000004016a5 <+805>:	cmp    %r13,%rbx
   0x00000000004016a8 <+808>:	jne    0x4016c5 <make_map_with_boost_assign()+837>
   0x00000000004016aa <+810>:	jmp    0x4016e8 <make_map_with_boost_assign()+872>
   0x00000000004016ac <+812>:	mov    0x8(%r12),%rbx
   0x00000000004016b1 <+817>:	add    $0x8,%r12
   0x00000000004016b5 <+821>:	data32 nopw %cs:0x0(%rax,%rax,1)
   0x00000000004016c0 <+832>:	cmp    %r13,%rbx
   0x00000000004016c3 <+835>:	je     0x4016e8 <make_map_with_boost_assign()+872>
   0x00000000004016c5 <+837>:	mov    %r14,%rdi
   0x00000000004016c8 <+840>:	mov    %r15,%rsi
   0x00000000004016cb <+843>:	mov    %rbx,%rdx
   0x00000000004016ce <+846>:	callq  0x405470 <std::__1::__tree<std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int>, std::__1::__map_value_compare<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int>, std::__1::less<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > >, true>, std::__1::allocator<std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int> > >::__emplace_hint_unique_impl<std::__1::pair<char const*, int>&>(std::__1::__tree_const_iterator<std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int>, std::__1::__tree_node<std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int>, void*>*, long>, std::__1::pair<char const*, int>&)>
   0x00000000004016d3 <+851>:	add    $0x10,%rbx
   0x00000000004016d7 <+855>:	mov    %rbx,%rax
   0x00000000004016da <+858>:	sub    (%r12),%rax
   0x00000000004016de <+862>:	cmp    $0x1000,%rax
   0x00000000004016e4 <+868>:	jne    0x4016c0 <make_map_with_boost_assign()+832>
   0x00000000004016e6 <+870>:	jmp    0x4016ac <make_map_with_boost_assign()+812>
   0x00000000004016e8 <+872>:	lea    0x8(%rsp),%rdi
   0x00000000004016ed <+877>:	callq  0x402bf0 <std::__1::__deque_base<std::__1::pair<char const*, int>, std::__1::allocator<std::__1::pair<char const*, int> > >::~__deque_base()>
   0x00000000004016f2 <+882>:	mov    %r14,%rax
   0x00000000004016f5 <+885>:	add    $0x40,%rsp
   0x00000000004016f9 <+889>:	pop    %rbx
   0x00000000004016fa <+890>:	pop    %r12
   0x00000000004016fc <+892>:	pop    %r13
   0x00000000004016fe <+894>:	pop    %r14
   0x0000000000401700 <+896>:	pop    %r15
   0x0000000000401702 <+898>:	retq   
   0x0000000000401703 <+899>:	jmp    0x40170b <make_map_with_boost_assign()+907>
   0x0000000000401705 <+901>:	jmp    0x40170b <make_map_with_boost_assign()+907>
   0x0000000000401707 <+903>:	jmp    0x40170b <make_map_with_boost_assign()+907>
   0x0000000000401709 <+905>:	jmp    0x40170b <make_map_with_boost_assign()+907>
   0x000000000040170b <+907>:	mov    %rax,%rbx
   0x000000000040170e <+910>:	jmp    0x40171f <make_map_with_boost_assign()+927>
   0x0000000000401710 <+912>:	mov    %rax,%rbx
   0x0000000000401713 <+915>:	mov    0x8(%r14),%rsi
   0x0000000000401717 <+919>:	mov    %r14,%rdi
   0x000000000040171a <+922>:	callq  0x402d40 <std::__1::__tree<std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int>, std::__1::__map_value_compare<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int>, std::__1::less<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > >, true>, std::__1::allocator<std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int> > >::destroy(std::__1::__tree_node<std::__1::__value_type<std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> >, int>, void*>*)>
   0x000000000040171f <+927>:	lea    0x8(%rsp),%rdi
   0x0000000000401724 <+932>:	callq  0x402bf0 <std::__1::__deque_base<std::__1::pair<char const*, int>, std::__1::allocator<std::__1::pair<char const*, int> > >::~__deque_base()>
   0x0000000000401729 <+937>:	mov    %rbx,%rdi
   0x000000000040172c <+940>:	callq  0x4011c0 <_Unwind_Resume@plt>
*/


std::map<std::string, int> make_map_manually ()
{
    std::map<std::string, int> retval;
    retval.emplace("<", 1);
    retval.emplace("<=",2);
    retval.emplace(">", 3);
    retval.emplace(">=",4);
    retval.emplace("=", 5);
    retval.emplace("<>",6);
    return retval;
}

/* TODO!!!
*/


int main()
{
    {
        std::map<std::string, int> op = make_map_with_boost_yap();

        std::cout << "\"<\"  --> " << op["<"] << std::endl;
        std::cout << "\"<=\" --> " << op["<="] << std::endl;
        std::cout << "\">\"  --> " << op[">"] << std::endl;
        std::cout << "\">=\" --> " << op[">="] << std::endl;
        std::cout << "\"=\"  --> " << op["="] << std::endl;
        std::cout << "\"<>\" --> " << op["<>"] << std::endl;
    }

    {
        std::map<std::string, int> op = make_map_with_boost_assign();

        std::cout << "\"<\"  --> " << op["<"] << std::endl;
        std::cout << "\"<=\" --> " << op["<="] << std::endl;
        std::cout << "\">\"  --> " << op[">"] << std::endl;
        std::cout << "\">=\" --> " << op[">="] << std::endl;
        std::cout << "\"=\"  --> " << op["="] << std::endl;
        std::cout << "\"<>\" --> " << op["<>"] << std::endl;
    }

    {
        std::map<std::string, int> op = make_map_manually();

        std::cout << "\"<\"  --> " << op["<"] << std::endl;
        std::cout << "\"<=\" --> " << op["<="] << std::endl;
        std::cout << "\">\"  --> " << op[">"] << std::endl;
        std::cout << "\">=\" --> " << op[">="] << std::endl;
        std::cout << "\"=\"  --> " << op["="] << std::endl;
        std::cout << "\"<>\" --> " << op["<>"] << std::endl;
    }

    return 0;
}
