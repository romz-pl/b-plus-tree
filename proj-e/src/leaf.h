#ifndef ROMZ_BTREE_LEAF_H
#define ROMZ_BTREE_LEAF_H

#include <utility>
#include <vector>
#include "node.h"

namespace btree
{


template< typename Key, typename Data >
class Leaf : public Node< Key, Data >
{
public:

    typedef typename Node< Key, Data >::value_type               value_type;
    typedef typename Node< Key, Data >::mutable_value_type       mutable_value_type;
    typedef typename Node< Key, Data >::key_type                 key_type;
    typedef typename Node< Key, Data >::reference                reference;
    typedef typename Node< Key, Data >::const_reference          const_reference;

public:
    enum
    {
        kValueSize = sizeof( Key ) + sizeof( Data ),
        kTargetNodeSize = 256,

        // Compute how many values we can fit onto a leaf node.
        kNodeTargetValues = ( kTargetNodeSize - sizeof( Node< Key, Data > ) ) / kValueSize,

        // We need a minimum of 3 values per internal node in order to perform
        // splitting (1 value for the two nodes involved in the split and 1 value
        // propagated to the parent as the delimiter for the split).
        kNodeValues = kNodeTargetValues >= 3 ? kNodeTargetValues : 3
    };

public:
    Leaf( );
    virtual ~Leaf() = default;

    void init( Internal< Key, Data > *parent, size_t max_count );

    const key_type& key( size_t i ) const;
    reference value( size_t i );
    const_reference value( size_t i ) const;
    mutable_value_type* mutable_value( size_t i );

    void value_swap( size_t i, Leaf *x, size_t j );

    void swap( Leaf *x );

    virtual Leaf< Key, Data >* get_leaf();
    virtual const Leaf< Key, Data >* get_leaf() const;


// protected:
    void value_init( size_t i );
    void value_init( size_t i, const value_type &x );
    void value_destroy( size_t i );

private:
    // The array of values. Only the first count of these values have been
    // constructed and are valid.
    std::vector< mutable_value_type > m_values;
};

//
//
//
template< typename K, typename D >
Leaf< K, D >::Leaf(  )
{
    this->m_leaf = true;
}

//
//
//
template< typename K, typename D >
void Leaf< K, D >::init( Internal< K, D > *parent, size_t max_count )
{
    Node< K, D >::init( parent, max_count );

    this->m_leaf = true;
    m_values.resize( max_count );
}

//
// Getters for the key/value at position i in the node.
//
template< typename K, typename D >
const typename Leaf< K, D >::key_type&
Leaf< K, D >::key( size_t i ) const
{
    return m_values[ i ].first;
}

//
//
//
template< typename K, typename D >
typename Leaf< K, D >::reference
Leaf< K, D >::value( size_t i )
{
    return reinterpret_cast< reference >( m_values[ i ] );
}

//
//
//
template< typename K, typename D >
typename Leaf< K, D >::const_reference
Leaf< K, D >::value( size_t i ) const
{
    return reinterpret_cast< const_reference >( m_values[ i ] );
}

//
//
//
template< typename K, typename D >
typename Leaf< K, D >::mutable_value_type*
Leaf< K, D >::mutable_value( size_t i )
{
    return &m_values[ i ];
}


//
//
//
template< typename K, typename D >
void Leaf< K, D >::value_init( size_t /*i*/ )
{
    // new( &m_values[ i ] ) mutable_value_type;
}

//
// Swap value i in this node with value j in node x.
//
template< typename K, typename D >
void Leaf< K, D >::value_swap( size_t i, Leaf *x, size_t j )
{
    std::swap( this->mutable_value( i )->first, x->mutable_value( j )->first );
    std::swap( this->mutable_value( i )->second, x->mutable_value( j )->second );
}

//
//
//
template< typename K, typename D >
void Leaf< K, D >::value_init( size_t i, const value_type &x )
{
    // new( &m_values[ i ] ) mutable_value_type( x );

    m_values[ i ] = x;
}

//
//
//
template< typename K, typename D >
void Leaf< K, D >::value_destroy( size_t /*i*/ )
{
    // m_values[ i ].~mutable_value_type();
}

//
//
//
template< typename K, typename D >
Leaf< K, D >* Leaf< K, D >::get_leaf()
{
    return this;
}

//
//
//
template< typename K, typename D >
const Leaf< K, D >* Leaf< K, D >::get_leaf() const
{
    return this;
}

//
// Swap the contents of "this" and "src".
//
template< typename K, typename D >
void Leaf< K, D >::swap( Leaf *x )
{

}

}

#endif
