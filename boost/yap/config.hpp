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

#endif
