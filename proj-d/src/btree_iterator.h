#ifndef ROMZ_BTREE_BTREE_ITERATOR_H
#define ROMZ_BTREE_BTREE_ITERATOR_H

namespace btree
{

//
//
//
template< typename Node, typename Reference, typename Pointer >
struct btree_iterator
{
    typedef typename Node::key_type                     key_type;
    typedef typename Node::size_type                    size_type;
    typedef typename Node::difference_type              difference_type;
    typedef typename Node::params_type                  params_type;
    typedef Node                                        node_type;
    typedef typename std::remove_const< Node >::type    normal_node;
    typedef const Node                                  const_node;
    typedef typename params_type::value_type            value_type;
    typedef typename params_type::pointer               normal_pointer;
    typedef typename params_type::reference             normal_reference;
    typedef typename params_type::const_pointer         const_pointer;
    typedef typename params_type::const_reference       const_reference;
    typedef Pointer                                     pointer;
    typedef Reference                                   reference;
    typedef std::bidirectional_iterator_tag             iterator_category;

    typedef btree_iterator< normal_node, normal_reference, normal_pointer>  iterator;
    typedef btree_iterator< const_node, const_reference, const_pointer>     const_iterator;
    typedef btree_iterator< Node, Reference, Pointer >                      self_type;

    btree_iterator();
    btree_iterator( Node *n, int p );
    btree_iterator( const iterator &x );

    void increment();
    void increment_by( int count );
    void increment_slow();

    void decrement();
    void decrement_slow();

    bool operator==( const const_iterator &x ) const;
    bool operator!=( const const_iterator &x ) const;

    const key_type& key() const;

    reference operator*() const;
    pointer operator->() const;

    self_type& operator++();
    self_type& operator--();

    self_type operator++( int );
    self_type operator--( int );

    // The node in the tree the iterator is pointing at.
    Node *node;

    // The position within the node of the tree the iterator is pointing at.
    int position;
};

//
//
//
template< typename N, typename R, typename P >
btree_iterator< N, R, P >::btree_iterator()
    : node( nullptr )
    , position( -1 )
{
}

//
//
//
template< typename N, typename R, typename P >
btree_iterator< N, R, P >::btree_iterator( N *n, int p )
    : node( n )
    , position( p )
{
}

//
//
//
template< typename N, typename R, typename P >
btree_iterator< N, R, P >::btree_iterator( const iterator &x )
  : node( x.node )
  , position( x.position )
{
}

//
// Increment/decrement the iterator.
//
template< typename N, typename R, typename P >
void btree_iterator< N, R, P >::increment()
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
template< typename N, typename R, typename P >
void btree_iterator< N, R, P >::increment_slow()
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
        node = node->child( position + 1 );
        while( !node->leaf() )
        {
            node = node->child( 0 );
        }
        position = 0;
    }
}

//
//
//
template< typename N, typename R, typename P >
void btree_iterator< N, R, P >::increment_by( int count )
{
    while( count > 0 )
    {
        if( node->leaf() )
        {
            int rest = node->count() - position;
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
template< typename N, typename R, typename P >
void btree_iterator< N, R, P >::decrement()
{
    if( node->leaf() && --position >= 0 )
    {
        return;
    }
    decrement_slow();
}

//
//
//
template< typename N, typename R, typename P >
void btree_iterator< N, R, P >::decrement_slow()
{
    if( node->leaf() )
    {
        assert( position <= -1 );
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
        node = node->child( position );
        while( !node->leaf() )
        {
            node = node->child( node->count() );
        }
        position = node->count() - 1;
    }
}

//
//
//
template< typename N, typename R, typename P >
bool btree_iterator< N, R, P >::operator==( const const_iterator &x ) const
{
    return node == x.node && position == x.position;
}

//
//
//
template< typename N, typename R, typename P >
bool btree_iterator< N, R, P >::operator!=( const const_iterator &x ) const
{
    return node != x.node || position != x.position;
}

//
// Accessors for the key/value the iterator is pointing at.
//
template< typename N, typename R, typename P >
const typename btree_iterator< N, R, P >::key_type&
btree_iterator< N, R, P >::key() const
{
    return node->key( position );
}

//
//
//
template< typename N, typename R, typename P >
typename btree_iterator< N, R, P >::reference
btree_iterator< N, R, P >::operator*() const
{
    return node->value( position );
}

//
//
//
template< typename N, typename R, typename P >
typename btree_iterator< N, R, P >::pointer
btree_iterator< N, R, P >::operator->() const
{
    return &node->value( position );
}

//
//
//
template< typename N, typename R, typename P >
typename btree_iterator< N, R, P >::self_type&
btree_iterator< N, R, P >::operator++()
{
    increment();
    return *this;
}

//
//
//
template< typename N, typename R, typename P >
typename btree_iterator< N, R, P >::self_type&
btree_iterator< N, R, P >::operator--()
{
    decrement();
    return *this;
}

//
//
//
template< typename N, typename R, typename P >
typename btree_iterator< N, R, P >::self_type
btree_iterator< N, R, P >::operator++( int )
{
    self_type tmp = *this;
    ++*this;
    return tmp;
}

//
//
//
template< typename N, typename R, typename P >
typename btree_iterator< N, R, P >::self_type
btree_iterator< N, R, P >::operator--( int )
{
    self_type tmp = *this;
    --*this;
    return tmp;
}

}


#endif
