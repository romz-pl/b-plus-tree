#include <gtest/gtest.h>
#include <btree_swap_helper.h>
#include <string>

//
//
//
TEST(btree_swap_helper, string)
{
    std::string a( "ala" );
    const std::string aOrg = a;
    std::string b( "xyz" );
    const std::string bOrg = b;

    btree::btree_swap_helper( a, b );

    EXPECT_EQ( a, bOrg );
    EXPECT_EQ( b, aOrg );
}
