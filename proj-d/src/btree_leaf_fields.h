#ifndef ROMZ_BTREE_LEAF_FIELDS_H
#define ROMZ_BTREE_LEAF_FIELDS_H

#include <cstring>
#include "btree_base_fields.h"

namespace btree
{

template< typename Params >
class btree_node;

template< typename Params >
class btree_leaf_fields : public btree_base_fields< Params >
{
public:

    typedef Params                                  params_type;
    typedef typename Params::value_type             value_type;
    typedef typename Params::mutable_value_type     mutable_value_type;
    typedef typename Params::key_type               key_type;
    typedef typename Params::reference              reference;
    typedef typename Params::const_reference        const_reference;

public:
    enum
    {
        kValueSize = params_type::kValueSize,
        kTargetNodeSize = params_type::kTargetNodeSize,

        // Compute how many values we can fit onto a leaf node.
        kNodeTargetValues = ( kTargetNodeSize - sizeof( btree_base_fields< Params > ) ) / kValueSize,

        // We need a minimum of 3 values per internal node in order to perform
        // splitting (1 value for the two nodes involved in the split and 1 value
        // propagated to the parent as the delimiter for the split).
        kNodeValues = kNodeTargetValues >= 3 ? kNodeTargetValues : 3
    };

public:
    const key_type& key( int i ) const;
    reference value( int i );
    const_reference value( int i ) const;
    mutable_value_type* mutable_value( int i );

    void value_swap( int i, btree_leaf_fields *x, int j );

    void init( btree_node< Params > *parent, int max_count );

// protected:
    void value_init( int i );
    void value_init( int i, const value_type &x );
    void value_destroy( int i );

private:
    // The array of values. Only the first count of these values have been
    // constructed and are valid.
    mutable_value_type m_values[ kNodeValues ];
};

//
// Getters for the key/value at position i in the node.
//
template< typename P >
const typename btree_leaf_fields< P >::key_type&
btree_leaf_fields< P >::key( int i ) const
{
    return params_type::key( m_values[ i ] );
}

//
//
//
template< typename P >
typename btree_leaf_fields< P >::reference
btree_leaf_fields< P >::value( int i )
{
    return reinterpret_cast< reference >( m_values[ i ] );
}

//
//
//
template< typename P >
typename btree_leaf_fields< P >::const_reference
btree_leaf_fields< P >::value( int i ) const
{
    return reinterpret_cast< const_reference >( m_values[ i ] );
}

//
//
//
template< typename P >
typename btree_leaf_fields< P >::mutable_value_type*
btree_leaf_fields< P >::mutable_value( int i )
{
    return &m_values[ i ];
}


//
//
//
template< typename P >
void btree_leaf_fields< P >::value_init( int i )
{
    new( &m_values[ i ] ) mutable_value_type;
}

//
// Swap value i in this node with value j in node x.
//
template< typename P >
void btree_leaf_fields< P >::value_swap( int i, btree_leaf_fields *x, int j )
{
    params_type::swap( this->mutable_value( i ), x->mutable_value( j ) );
}

//
//
//
template< typename P >
void btree_leaf_fields< P >::value_init( int i, const value_type &x )
{
    new( &m_values[ i ] ) mutable_value_type( x );
}

//
//
//
template< typename P >
void btree_leaf_fields< P >::value_destroy( int i )
{
    m_values[ i ].~mutable_value_type();
}

//
//
//
template< typename P >
void btree_leaf_fields< P >::init( btree_node< P > *parent, int max_count )
{
    btree_base_fields< P >::init( parent, max_count );

#ifndef NDEBUG
    std::memset( &m_values, 0, max_count * sizeof( value_type ) );
#endif
}

}

#endif
