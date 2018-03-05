#include <gtest/gtest.h>
#include <btree_common_params.h>
#include <string>
#include <type_traits>

//
//
//
TEST(btree_common_params, int)
{
    using Key = int;
    using Data = int;
    using Compare = std::less< Key >;
    using Alloc = std::allocator< std::pair< const Key, Data > >;
    const int TargetNodeSize = 300;
    const int ValueSize = 20;

    typedef btree::btree_common_params< Key, Compare, Alloc, TargetNodeSize, ValueSize > theType;

    bool v = btree::btree_is_key_compare_to< Compare >::value;
    EXPECT_FALSE( v );

    v = std::is_same< theType::key_compare, Compare >::value;
    EXPECT_FALSE( v );

    v = std::is_convertible< theType::key_compare, Compare >::value;
    EXPECT_TRUE( v );

    v = std::is_same< theType::key_compare, btree::btree_key_compare_to_adapter< Compare > >::value;
    EXPECT_TRUE( v );

    v = theType::is_key_compare_to::value;
    EXPECT_FALSE( v );

    v = std::is_same< theType::allocator_type, Alloc >::value;
    EXPECT_TRUE( v );

    v = std::is_same< theType::key_type, Key >::value;
    EXPECT_TRUE( v );

    v = ( theType::kNodeValueSpace / ValueSize ) >= 256;
    EXPECT_FALSE( v );

    v = std::is_same< theType::node_count_type, uint8_t >::value;
    EXPECT_TRUE( v );
}

//
//
//
TEST(btree_common_params, string)
{
    using Key = std::string;
    using Data = int;
    using Compare = std::less< Key >;
    using Alloc = std::allocator< std::pair< const Key, Data > >;
    const int TargetNodeSize = 300;
    const int ValueSize = 20;

    typedef btree::btree_common_params< Key, Compare, Alloc, TargetNodeSize, ValueSize > theType;

    bool v = btree::btree_is_key_compare_to< Compare >::value;
    EXPECT_FALSE( v );

    v = std::is_same< theType::key_compare, Compare >::value;
    EXPECT_FALSE( v );

    v = std::is_convertible< theType::key_compare, btree::btree_key_compare_to_tag >::value;
    EXPECT_TRUE( v );

    v = std::is_same< theType::key_compare, btree::btree_key_compare_to_adapter< Compare > >::value;
    EXPECT_TRUE( v );

    v = theType::is_key_compare_to::value;
    EXPECT_TRUE( v );

    v = std::is_same< theType::allocator_type, Alloc >::value;
    EXPECT_TRUE( v );

    v = std::is_same< theType::key_type, Key >::value;
    EXPECT_TRUE( v );

    v = ( theType::kNodeValueSpace / ValueSize ) >= 256;
    EXPECT_FALSE( v );

    v = std::is_same< theType::node_count_type, uint8_t >::value;
    EXPECT_TRUE( v );
}
