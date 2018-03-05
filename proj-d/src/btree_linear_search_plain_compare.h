#ifndef ROMZ_BTREE_LINEAR_SEARCH_PLAIN_COMPARE_H
#define ROMZ_BTREE_LINEAR_SEARCH_PLAIN_COMPARE_H


#include "btree_upper_bound_adapter.h"

namespace btree
{

//
// Dispatch helper class for using linear search with plain compare.
//
template< typename K, typename N, typename Compare >
struct btree_linear_search_plain_compare
{
    static int lower_bound( const K &k, const N &n, Compare comp )
    {
        return n.linear_search_plain_compare( k, 0, n.count(), comp );
    }

    static int upper_bound( const K &k, const N &n, Compare comp )
    {
        typedef btree_upper_bound_adapter< K, Compare > upper_compare;
        return n.linear_search_plain_compare( k, 0, n.count(), upper_compare( comp ) );
    }
};

}

#endif
