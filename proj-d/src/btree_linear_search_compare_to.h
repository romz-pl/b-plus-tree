#ifndef ROMZ_BTREE_LINEAR_SEARCH_COMPARE_TO_H
#define ROMZ_BTREE_LINEAR_SEARCH_COMPARE_TO_H

#include "btree_upper_bound_adapter.h"
#include "btree_key_comparer.h"

namespace btree
{

//
// Dispatch helper class for using linear search with compare-to
//
template< typename K, typename N, typename CompareTo >
struct btree_linear_search_compare_to
{
    static int lower_bound( const K &k, const N &n, CompareTo comp )
    {
        return n.linear_search_compare_to( k, 0, n.count(), comp );
    }

    static int upper_bound( const K &k, const N &n, CompareTo comp )
    {
        typedef btree_upper_bound_adapter< K, btree_key_comparer< K, CompareTo, true > > upper_compare;
        return n.linear_search_plain_compare( k, 0, n.count(), upper_compare( comp ) );
    }
};

}


#endif
