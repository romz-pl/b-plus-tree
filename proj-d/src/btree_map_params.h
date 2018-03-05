#ifndef ROMZ_BTREE_MAP_PARAMS_H
#define ROMZ_BTREE_MAP_PARAMS_H

#include <utility>

#include "btree_common_params.h"
#include "btree_swap_helper.h"

namespace btree
{

//
// A parameters structure for holding the type parameters for a btree_map.
//
template< typename Key, typename Data, typename Compare, typename Alloc, int TargetNodeSize >
struct btree_map_params
        : public btree_common_params< Key, Compare, Alloc, TargetNodeSize, sizeof( Key ) + sizeof( Data ) >
{
    typedef Data                              data_type;
    typedef Data                              mapped_type;
    typedef std::pair< const Key, data_type > value_type;
    typedef std::pair< Key, data_type >       mutable_value_type;
    typedef value_type*                       pointer;
    typedef const value_type*                 const_pointer;
    typedef value_type&                       reference;
    typedef const value_type&                 const_reference;

    enum
    {
        kValueSize = sizeof( Key ) + sizeof( data_type ),
    };

    static const Key& key( const value_type &x )
    {
        return x.first;
    }

    static const Key& key( const mutable_value_type &x )
    {
        return x.first;
    }

    static void swap( mutable_value_type *a, mutable_value_type *b )
    {
        btree_swap_helper( a->first, b->first );
        btree_swap_helper( a->second, b->second );
    }
};

}


#endif
