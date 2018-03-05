#include <gtest/gtest.h>
#include <internal.h>


//
//
//
TEST(Internal, init)
{
    using Key = int;
    using Data = int;

    btree::Internal< Key, Data > internal( nullptr );

    const Key key = 2;
    const Data data = 3;
    const btree::Leaf< Key, Data >::value_type x( key, data );

    internal.insert_value( 0, x );
    EXPECT_EQ( internal.value( 0 ), x );
    EXPECT_EQ( internal.count( ), 1U );

    btree::Internal< Key, Data > c0( nullptr );
    btree::Internal< Key, Data > c1( nullptr );

    internal.set_child( 0, &c0 );
    internal.set_child( 1, &c1 );

    const btree::Leaf< Key, Data >::value_type y( key + 1, data + 1 );
    internal.insert_value( 0, y );
    EXPECT_EQ( internal.value( 0 ), y );
    EXPECT_EQ( internal.value( 1 ), x );
    EXPECT_EQ( internal.count( ), 2U );

    btree::Internal< Key, Data > c2( nullptr );
    internal.set_child( 1, &c2 );


    internal.remove_value( 0 );
    EXPECT_EQ( internal.value( 0 ), x );
    EXPECT_EQ( internal.count( ), 1U );

    internal.remove_value( 0 );
    EXPECT_EQ( internal.count( ), 0U );

}
