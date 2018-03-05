#include <gtest/gtest.h>
#include <btree_base_fields.h>
#include <btree_map_params.h>
#include <btree_node.h>

//
//
//
TEST(btree_base_fields, init)
{
    using Key = int;
    using Data = int;
    using Compare = std::less< Key >;
    using Alloc = std::allocator< std::pair< const Key, Data > >;
    const int TargetNodeSize = 300;

    typedef btree::btree_map_params< Key, Data, Compare, Alloc, TargetNodeSize > params;

    btree::btree_base_fields< params > base;

    btree::btree_node< params >* parent = nullptr;
    const btree::btree_base_fields< params >::field_type max_count = 23;
    base.init( parent, max_count );

    EXPECT_TRUE( base.leaf() );

    const btree::btree_base_fields< params >::field_type v = 1;
    base.set_position( v );
    EXPECT_EQ( base.position(), v );

    base.set_count( v );
    EXPECT_EQ( base.count(), v );

    EXPECT_EQ( base.max_count(), max_count );

    EXPECT_EQ( base.parent(), parent );

    // EXPECT_TRUE( base.is_root() );

}
