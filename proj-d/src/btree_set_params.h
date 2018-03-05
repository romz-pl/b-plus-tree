#ifndef ROMZ_BTREE_SET_PARAMS_H
#define ROMZ_BTREE_SET_PARAMS_H

#include "btree_common_params.h"
#include "btree_swap_helper.h"

namespace btree
{

//
// A parameters structure for holding the type parameters for a btree_set.
//
template< typename Key, typename Compare, typename Alloc, int TargetNodeSize >
struct btree_set_params
    : public btree_common_params< Key, Compare, Alloc, TargetNodeSize, sizeof( Key ) >
{
    typedef std::false_type      data_type;
    typedef std::false_type      mapped_type;
    typedef Key                  value_type;
    typedef value_type           mutable_value_type;
    typedef value_type*          pointer;
    typedef const value_type*    const_pointer;
    typedef value_type&          reference;
    typedef const value_type&    const_reference;

    enum
    {
        kValueSize = sizeof( Key ),
    };

    static const Key& key( const value_type &x )
    {
        return x;
    }

    static void swap( mutable_value_type *a, mutable_value_type *b )
    {
        btree_swap_helper< mutable_value_type >( *a, *b );
    }
};

}


#endif
