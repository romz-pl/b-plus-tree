#include <gtest/gtest.h>
#include <leaf.h>


//
//
//
TEST(Leaf, init)
{
    using Key = int;
    using Data = int;

    const size_t max_count = 230;
    btree::Leaf< Key, Data > leaf( nullptr, max_count );

    const Key key = 2;
    const Data data = 3;
    const btree::Leaf< Key, Data >::value_type x( key, data );

    leaf.value_init( 0, x );
    EXPECT_EQ( leaf.key( 0 ), key );
    EXPECT_EQ( leaf.value( 0 ), x );

    const btree::Leaf< Key, Data >::value_type y( key + 1, data + 1 );
    leaf.value_init( 1, y );
    EXPECT_EQ( leaf.value( 0 ), x );
    EXPECT_EQ( leaf.value( 1 ), y );

    EXPECT_TRUE( leaf.get_leaf() != nullptr );


    btree::Leaf< Key, Data > bb( nullptr, max_count );
    leaf.value_swap( 0, &bb, 0 );
    leaf.value_swap( 1, &bb, 1 );

    EXPECT_EQ( bb.value( 0 ), x );
    EXPECT_EQ( bb.value( 1 ), y );

}
