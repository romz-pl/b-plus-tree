#include <gtest/gtest.h>
#include <node.h>


//
//
//
TEST(Node, init)
{
    using Key = int;
    using Data = int;

    const size_t max_count = 230;
    btree::Node< Key, Data > node( nullptr, max_count );

    EXPECT_TRUE( node.leaf() );

    {
        const uint16_t v = 2;
        node.set_position( v );
        EXPECT_EQ( node.position(), v );
    }

    {
        const uint16_t v = 3;
        node.set_count( v );
        EXPECT_EQ( node.count(), v );
    }


    EXPECT_EQ( node.max_count(), max_count );


    // EXPECT_TRUE( node.is_root() );

}

