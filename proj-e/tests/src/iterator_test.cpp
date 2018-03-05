#include <gtest/gtest.h>
#include <iterator.h>


//
//
//
TEST(Iterator, init)
{
    using Key = int;
    using Data = int;


    const size_t max_count = 230;
    btree::Leaf< Key, Data > leaf( nullptr, max_count );
    const Key key = 2;
    const Data data = 3;
    const btree::Leaf< Key, Data >::value_type x( key, data );
    leaf.value_init( 0, x );
    leaf.value_init( 1, x );
    leaf.value_init( 2, x );
    leaf.value_init( 3, x );
    leaf.value_init( 4, x );
    leaf.set_count( 5 );


    btree::Iterator< Key, Data > iter( &leaf, 0 );

    iter.increment();
    iter.increment_by( 3 );

    iter.decrement();

    EXPECT_TRUE( iter == iter );
    EXPECT_FALSE( iter != iter );

    btree::Iterator< Key, Data > w( iter );
    EXPECT_TRUE( iter == w );
    w++;
    EXPECT_FALSE( iter == w );
    w--;
    EXPECT_TRUE( iter == w );
    ++w;
    EXPECT_FALSE( iter == w );
    --w;
    EXPECT_TRUE( iter == w );

    // *www;
    // www->first;
}


