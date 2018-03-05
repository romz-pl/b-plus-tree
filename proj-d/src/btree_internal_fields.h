#ifndef ROMZ_BTREE_INTERNAL_FIELDS_H
#define ROMZ_BTREE_INTERNAL_FIELDS_H

#include <cstring>
#include "btree_leaf_fields.h"

namespace btree
{

template< typename Params >
class btree_node;

template< typename Params >
class btree_internal_fields : public btree_leaf_fields< Params >
{
public:
    typedef typename Params::value_type         value_type;


public:
    btree_node< Params >* child( int i ) const;
    btree_node< Params >** mutable_child( int i );

    void insert_value( int i, const value_type &x );
    void remove_value( int i );

    void init( btree_node< Params > *parent );

private:
    // The array of child pointers. The keys in children_[i] are all less than
    // key(i). The keys in children_[i + 1] are all greater than key(i). There
    // are always count + 1 children.
    btree_node< Params > *m_children[ btree_leaf_fields< Params >::kNodeValues + 1 ];
};

//
// Getters/setter for the child at position i in the node.
//
template< typename P >
btree_node< P >* btree_internal_fields< P >::child( int i ) const
{
    return m_children[ i ];
}

//
//
//
template< typename P >
btree_node< P >** btree_internal_fields< P >::mutable_child( int i )
{
    return &m_children[ i ];
}


//
// Inserts the value x at position i, shifting all existing values and
// children at positions >= i to the right by 1.
//
template< typename P >
inline void btree_internal_fields< P >::insert_value( int i, const value_type &x )
{
    assert( i <= this->count() );
    this->value_init( this->count(), x );
    for( int j = this->count(); j > i; --j )
    {
        this->value_swap( j, this, j - 1 );
    }
    this->set_count( this->count() + 1 );

    if( !this->leaf() )
    {
        ++i;
        for( int j = this->count(); j > i; --j )
        {
            *mutable_child( j ) = child( j - 1 );
            child( j )->set_position( j );
        }
        *mutable_child( i ) = nullptr;
    }
}

//
// Removes the value at position i, shifting all existing values and children
// at positions > i to the left by 1.
//
template< typename P >
inline void btree_internal_fields< P >::remove_value( int i )
{
    if( !this->leaf() )
    {
        assert( child( i + 1 )->count() == 0 );
        for( int j = i + 1; j < this->count(); ++j )
        {
            *mutable_child( j ) = child( j + 1 );
            child( j )->set_position( j );
        }
        *mutable_child( this->count() ) = nullptr;
    }

    this->set_count( this->count() - 1 );
    for( ; i < this->count(); ++i )
    {
        this->value_swap( i, this, i + 1 );
    }
    this->value_destroy( i );
}


//
//
//
template< typename P >
void btree_internal_fields< P >::init( btree_node< P > *parent )
{
    btree_leaf_fields< P >::init( parent, btree_leaf_fields< P >::kNodeValues );
    this->m_leaf = false;

#ifndef NDEBUG
    std::memset( this->m_children, 0, sizeof( this->m_children ) );
#endif

}

}

#endif

