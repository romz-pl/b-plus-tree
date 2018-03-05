#ifndef ROMZ_BTREE_COMPARE_KEYS_H
#define ROMZ_BTREE_COMPARE_KEYS_H


#include "btree_key_comparer.h"
#include "btree_is_key_compare_to.h"

namespace btree
{

//
// A helper function to compare to keys using the specified compare
// functor. This dispatches to the appropriate btree_key_comparer comparison,
// depending on whether we have a compare-to functor or not (which depends on
// whether Compare is derived from btree_key_compare_to_tag).
//
template< typename Key, typename Compare >
static bool btree_compare_keys( const Compare &comp, const Key &x, const Key &y )
{
    typedef btree_key_comparer< Key, Compare, btree_is_key_compare_to< Compare >::value > key_comparer;
    return key_comparer::bool_compare( comp, x, y );
}

}

#endif

