#include <gtest/gtest.h>
#include <btree_leaf_fields.h>
#include <btree_map_params.h>

//
//
//
TEST(btree_leaf_fields, init)
{
    using Key = int;
    using Data = int;
    using Compare = std::less< Key >;
    using Alloc = std::allocator< std::pair< const Key, Data > >;
    const int TargetNodeSize = 300;

    typedef btree::btree_map_params< Key, Data, Compare, Alloc, TargetNodeSize > params;

    btree::btree_leaf_fields< params > leaf;

    btree::btree_node< params >* parent = nullptr;
    const int max_count = 23;
    leaf.init( parent, max_count );

    const Key key = 3;
    const Data data = 4;
    const btree::btree_leaf_fields< params >::value_type x( key, data );
    const int i = 2;
    leaf.value_init( i , x );
    EXPECT_EQ( leaf.value( i ), x );
    EXPECT_EQ( leaf.key( i ), key );

    leaf.value_destroy( i );

}
