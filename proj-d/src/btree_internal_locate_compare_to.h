#ifndef ROMZ_BTREE_INTERNAL_LOCATE_COMPARE_TO_H
#define ROMZ_BTREE_INTERNAL_LOCATE_COMPARE_TO_H

#include <utility>

namespace btree
{

//
// Dispatch helper class for using btree::internal_locate with compare-to.
//
struct btree_internal_locate_compare_to
{
    template< typename K, typename T, typename Iter >
    static std::pair< Iter, int > dispatch( const K &k, const T &t, Iter iter )
    {
        return t.internal_locate_compare_to(k, iter);
    }
};

}

#endif
