#ifndef BOOST_YAP_CONFIG_HPP_INCLUDED
#define BOOST_YAP_CONFIG_HPP_INCLUDED


#ifndef BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE
/** Controls whether a conversion operator template is defined for
    <code>expression<></code>.

    Default value is <code>0</code>.

    This is useful when you want to have <code>expression<></code>s
    auto-evaluated on assignment to non-<code>yap</code> types.  It can also
    wreak havok on your code due to unintentional implicit conversions.  Here
    be dragons. */
#define BOOST_YAP_CONVERSION_OPERATOR_TEMPLATE 0
#endif

#ifndef BOOST_NO_CONSTEXPR_IF
/** Indicates whether the compiler supports constexpr if.

    If the user does not define any value for this, we assume that the
    compiler does not have the necessary support.  Note that this is a
    temporary hack; this should eventually be a Boost-wide macro. */
#define BOOST_NO_CONSTEXPR_IF 1
#endif

#endif
