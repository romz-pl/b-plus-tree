#ifndef ROMZ_BTREE_ITERATOR_H
#define ROMZ_BTREE_ITERATOR_H

#include <type_traits>
#include <iterator>
#include <cassert>
#include <algorithm>
#include "internal.h"

namespace btree
{

//
//
//
template< typename Key, typename Data >
class Iterator
{
public:
    typedef typename Node< Key, Data >::key_type              key_type;
    typedef typename Node< Key, Data >::size_type             size_type;
    typedef typename Node< Key, Data >::difference_type       difference_type;
    typedef typename Node< Key, Data >::value_type            value_type;
    typedef typename Node< Key, Data >::pointer               pointer;
    typedef typename Node< Key, Data >::reference             reference;
    typedef typename Node< Key, Data >::const_pointer         const_pointer;
    typedef typename Node< Key, Data >::const_reference       const_reference;

    typedef Node< Key, Data >                                        node_type;
    typedef typename std::remove_const< Node< Key, Data > >::type    normal_node;
    typedef const Node< Key, Data >                                  const_node;

    typedef std::bidirectional_iterator_tag             iterator_category;

    typedef Iterator< Key, Data >           iterator;
    typedef const Iterator< Key, Data >     const_iterator;
    typedef Iterator< Key, Data >           self_type;

public:
    Iterator();
    Iterator( Node< Key, Data > *n, size_t p );
    Iterator( const iterator &x );

    void increment();
    void increment_by( size_t count );
    void decrement();


    bool operator==( const const_iterator &x ) const;
    bool operator!=( const const_iterator &x ) const;

    const key_type& key() const;

    reference operator*() const;
    pointer operator->() const;

    self_type& operator++();
    self_type& operator--();

    self_type operator++( int );
    self_type operator--( int );


private:
    void increment_slow();
    void decrement_slow();

public:

    // The node in the tree the iterator is pointing at.
    Node< Key, Data > *node;

    // The position within the node of the tree the iterator is pointing at.
    size_t position;
};

//
//
//
template< typename K, typename D >
Iterator< K, D >::Iterator()
    : node( nullptr )
    , position( 0 )
{
}

//
//
//
template< typename K, typename D >
Iterator< K, D >::Iterator( Node< K, D > *n, size_t p )
    : node( n )
    , position( p )
{
}

//
//
//
template< typename K, typename D >
Iterator< K, D >::Iterator( const iterator &x )
  : node( x.node )
  , position( x.position )
{
}

//
// Increment/decrement the iterator.
//
template< typename K, typename D >
void Iterator< K, D >::increment()
{
    if( node->leaf() && ++position < node->count() )
    {
        return;
    }
    increment_slow();
}



//
//
//
template< typename K, typename D >
void Iterator< K, D >::increment_slow()
{
    if( node->leaf() )
    {
        assert( position >= node->count() );
        self_type save( *this );
        while( position == node->count() && !node->is_root() )
        {
            assert( node->parent()->child( node->position() ) == node );
            position = node->position();
            node = node->parent();
        }
        if( position == node->count() )
        {
            *this = save;
        }
    }
    else
    {
        assert( position < node->count() );
        node = node->get_internal()->child( position + 1 );
        while( !node->leaf() )
        {
            node = node->get_internal()->child( 0 );
        }
        position = 0;
    }
}

//
//
//
template< typename K, typename D >
void Iterator< K, D >::increment_by( size_t count )
{
    while( count > 0 )
    {
        if( node->leaf() )
        {
            assert( node->count() >= position );
            size_t rest = node->count() - position;
            position += std::min( rest, count );
            count = count - rest;
            if( position < node->count() )
            {
                return;
            }
        }
        else
        {
            --count;
        }
        increment_slow();
    }
}

//
//
//
template< typename K, typename D >
void Iterator< K, D >::decrement()
{
    if( node->leaf() && position > 0 )
    {
        --position;
        return;
    }
    decrement_slow();
}

//
//
//
template< typename K, typename D >
void Iterator< K, D >::decrement_slow()
{
    if( node->leaf() )
    {
        assert( position == 0 );
        self_type save( *this );
        while( position < 0 && !node->is_root() )
        {
            assert( node->parent()->child( node->position() ) == node );
            position = node->position() - 1;
            node = node->parent();
        }
        if( position < 0 )
        {
            *this = save;
        }
    }
    else
    {
        assert( position >= 0 );
        node = node->get_internal()->child( position );
        while( !node->leaf() )
        {
            node = node->get_internal()->child( node->count() );
        }
        position = node->count() - 1;
    }
}

//
//
//
template< typename K, typename D >
bool Iterator< K, D >::operator==( const const_iterator &x ) const
{
    return node == x.node && position == x.position;
}

//
//
//
template< typename K, typename D >
bool Iterator< K, D >::operator!=( const const_iterator &x ) const
{
    return node != x.node || position != x.position;
}

//
// Accessors for the key/value the iterator is pointing at.
//
template< typename K, typename D >
const typename Iterator< K, D >::key_type&
Iterator< K, D >::key() const
{
    if( node->leaf() )
    {
        return node->get_leaf()->key( position );
    }
    else
    {
        return node->get_internal()->key( position );
    }
}

//
//
//
template< typename K, typename D >
typename Iterator< K, D >::reference
Iterator< K, D >::operator*() const
{
    if( node->leaf() )
    {
        return node->get_leaf()->value( position );
    }
    else
    {
        return node->get_internal()->value( position );
    }
}

//
//
//
template< typename K, typename D >
typename Iterator< K, D >::pointer
Iterator< K, D >::operator->() const
{
    if( node->leaf() )
    {
        return &node->get_leaf()->value( position );
    }
    else
    {
        return &node->get_internal()->value( position );
    }
}

//
//
//
template< typename K, typename D >
typename Iterator< K, D >::self_type&
Iterator< K, D >::operator++()
{
    increment();
    return *this;
}

//
//
//
template< typename K, typename D >
typename Iterator< K, D >::self_type&
Iterator< K, D >::operator--()
{
    decrement();
    return *this;
}

//
//
//
template< typename K, typename D >
typename Iterator< K, D >::self_type
Iterator< K, D >::operator++( int )
{
    self_type tmp = *this;
    ++*this;
    return tmp;
}

//
//
//
template< typename K, typename D >
typename Iterator< K, D >::self_type
Iterator< K, D >::operator--( int )
{
    self_type tmp = *this;
    --*this;
    return tmp;
}

}


#endif
