#ifndef ROMZ_BTREE_UPPER_BOUND_ADAPTER_H
#define ROMZ_BTREE_UPPER_BOUND_ADAPTER_H

namespace btree
{

//
// An adapter class that converts a lower-bound compare into an upper-bound
// compare.
//
template< typename Key, typename Compare >
struct btree_upper_bound_adapter : public Compare
{
    btree_upper_bound_adapter( Compare c ) : Compare( c ) { }

    bool operator()( const Key &a, const Key &b ) const
    {
        return !static_cast< const Compare& >( *this )( b, a );
    }
};

}


#endif

