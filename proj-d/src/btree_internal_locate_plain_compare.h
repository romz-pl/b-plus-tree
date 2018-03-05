#ifndef ROMZ_BTREE_INTERNAL_LOCATE_PLAIN_COMPARE_H
#define ROMZ_BTREE_INTERNAL_LOCATE_PLAIN_COMPARE_H

#include <utility>

namespace btree
{

//
// Dispatch helper class for using btree::internal_locate with plain compare.
//
struct btree_internal_locate_plain_compare
{
    template< typename K, typename T, typename Iter >
    static std::pair< Iter, int  > dispatch( const K &k, const T &t, Iter iter )
    {
        return t.internal_locate_plain_compare( k, iter );
    }
};

}

#endif
