#include <iostream>
#include "btree.h"

int main( )
{

    using Key = int;
    using Data = int;

    btree::Btree< Key, Data > b;

    const Key key = 2;
    const Data data = 3;
    btree::Btree< Key, Data >::value_type x( key, data );

    b.insert_unique( x );

    std::cout << "OK\n";
    return 0;
}

