#ifndef ROMZ_BTREE_UPPER_BOUND_COMPARE_TO_ADAPTER_H
#define ROMZ_BTREE_UPPER_BOUND_COMPARE_TO_ADAPTER_H

namespace btree
{

//
//
//
template< typename Key, typename CompareTo >
struct btree_upper_bound_compare_to_adapter : public CompareTo
{
    btree_upper_bound_compare_to_adapter(CompareTo c) : CompareTo(c) {}

    int operator()( const Key &a, const Key &b ) const
    {
        return static_cast< const CompareTo& >( *this )( b, a );
    }
};

}

#endif
