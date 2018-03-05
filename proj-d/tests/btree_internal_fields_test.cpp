#include <gtest/gtest.h>
#include <btree_internal_fields.h>
#include <btree_map_params.h>
#include <btree_node.h>

//
//
//
TEST(btree_internal_fields, init)
{
    using Key = int;
    using Data = int;
    using Compare = std::less< Key >;
    using Alloc = std::allocator< std::pair< const Key, Data > >;
    const int TargetNodeSize = 300;

    typedef btree::btree_map_params< Key, Data, Compare, Alloc, TargetNodeSize > params;

    btree::btree_internal_fields< params > internal;

    btree::btree_node< params >* parent = nullptr;
    internal.init( parent );

    const Key key = 3;
    const Data data = 4;
    const btree::btree_leaf_fields< params >::value_type x( key, data );

    /*
    std::cout << "A\n" << std::flush;
    internal.insert_value( 0 , x );
    EXPECT_EQ( internal.value( 0 ), x );
    EXPECT_EQ( internal.count( ), 1 );

    std::cout << "A\n" << std::flush;
    internal.insert_value( 0 , x );
    std::cout << "A\n" << std::flush;
    EXPECT_EQ( internal.value( 0 ), x );
    EXPECT_EQ( internal.count( ), 2 );

    std::cout << "A\n" << std::flush;
    internal.remove_value( 0 );
    EXPECT_EQ( internal.count( ), 1 );

    std::cout << "A\n" << std::flush;
    internal.remove_value( 0 );
    EXPECT_EQ( internal.count( ), 0 );

    */
}

