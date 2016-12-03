#include <boost/yap/expression.hpp>

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


// TODO: Add this to perf dir, and look at the generated code for this and the
// equivalent Boost.Assign.
int main()
{
    // Initialize a map:
    std::map<std::string, int> op =
        map_list_of
            ("<", 1)
            ("<=",2)
            (">", 3)
            (">=",4)
            ("=", 5)
            ("<>",6)
        ;

    std::cout << "\"<\"  --> " << op["<"] << std::endl;
    std::cout << "\"<=\" --> " << op["<="] << std::endl;
    std::cout << "\">\"  --> " << op[">"] << std::endl;
    std::cout << "\">=\" --> " << op[">="] << std::endl;
    std::cout << "\"=\"  --> " << op["="] << std::endl;
    std::cout << "\"<>\" --> " << op["<>"] << std::endl;

    return 0;
}
