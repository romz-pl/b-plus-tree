#ifndef ROMZ_BTREE_COMMON_PARAMS_H
#define ROMZ_BTREE_COMMON_PARAMS_H

#include <type_traits>
#include <cstddef>
#include <cstdint>

#include "btree_is_key_compare_to.h"
#include "btree_key_compare_to_adapter.h"

namespace btree
{

//
//
//
template< typename Key, typename Compare, typename Alloc, int TargetNodeSize, int ValueSize >
struct btree_common_params
{
    // If Compare is derived from btree_key_compare_to_tag then use it as the
    // key_compare type. Otherwise, use btree_key_compare_to_adapter<> which will
    // fall-back to Compare if we don't have an appropriate specialization.
    typedef typename std::conditional< btree_is_key_compare_to< Compare >::value,
                          Compare,
                          btree_key_compare_to_adapter< Compare > >::type key_compare;

    // A type which indicates if we have a key-compare-to functor or a plain old
    // key-compare functor.
    typedef btree_is_key_compare_to< key_compare > is_key_compare_to;

    typedef Alloc     allocator_type;
    typedef Key       key_type;
    typedef size_t    size_type;
    typedef ptrdiff_t difference_type;



    enum
    {
        kTargetNodeSize = TargetNodeSize,

        // Available space for values.  This is largest for leaf nodes,
        // which has overhead no fewer than two pointers.
        kNodeValueSpace = TargetNodeSize - 2 * sizeof( void* ),
    };

    // This is an integral type large enough to hold as many
    // ValueSize-values as will fit a node of TargetNodeSize bytes.
    typedef typename std::conditional< ( kNodeValueSpace / ValueSize ) >= 256,
                           uint16_t, uint8_t >::type node_count_type;


    static_assert( TargetNodeSize > 2 * sizeof( void* ), "TargetNodeSize too small" );
    static_assert( ValueSize > 0, "ValueSize must be greater than zero" );
    static_assert( kNodeValueSpace > ValueSize, "Not enought space for the Value" );
};

}

#endif
