#ifndef ROMZ_BTREE_INTERNAL_H
#define ROMZ_BTREE_INTERNAL_H


#include "leaf.h"

namespace btree
{



template< typename Key, typename Data >
class Internal : public Leaf< Key, Data >
{
public:
    typedef typename Leaf< Key, Data >::value_type         value_type;


public:
    Internal( );
    virtual ~Internal() = default;

    void init( Internal< Key, Data > *parent );


    Node< Key, Data >* child( size_t i ) const;
    Node< Key, Data >** mutable_child( size_t i );
    void set_child( size_t i, Node< Key, Data > *c );

    void insert_value( size_t i, const value_type &x );
    void remove_value( size_t i );

    void rebalance_right_to_left( Internal *sibling, size_t to_move );
    void rebalance_left_to_right( Internal *sibling, size_t to_move );

    void split( Internal *sibling, size_t insert_position );
    void merge( Internal *sibling );

    void swap( Internal *src );

    void destroy();

    virtual Internal< Key, Data >* get_internal();
    virtual const Internal< Key, Data >* get_internal() const;



private:
    // The array of child pointers.
    // The keys in m_children[i] are all less than key(i).
    // The keys in m_children[i + 1] are all greater than key(i).
    // There are always (count + 1) children.
    std::vector< Node< Key, Data >* > m_children;
};

//
//
//
template< typename K, typename D >
Internal< K, D >::Internal( )
{
    this->m_leaf = false;
}

//
//
//
template< typename K, typename D >
void Internal< K, D >::init( Internal< K, D > *parent )
{
    Leaf< K, D >::init( parent, static_cast< size_t >( Leaf< K, D >::kNodeValues ) );

    m_children.resize( Leaf< K, D >::kNodeValues + 1, nullptr );
    this->m_leaf = false;
}

//
// Getters/setter for the child at position i in the node.
//
template< typename K, typename D >
Node< K, D >* Internal< K, D >::child( size_t i ) const
{
    return m_children[ i ];
}

//
//
//
template< typename K, typename D >
Node< K, D >** Internal< K, D >::mutable_child( size_t i )
{
    return &m_children[ i ];
}

//
//
//
template< typename K, typename D >
void Internal< K, D >::set_child( size_t i, Node< K, D > *c )
{
    *this->mutable_child( i ) = c;
    c->m_parent = this;
    c->m_position = i;
}

//
// Inserts the value x at position i, shifting all existing values and
// children at positions >= i to the right by 1.
//
template< typename K, typename D >
void Internal< K, D >::insert_value( size_t i, const value_type &x )
{
    assert( i <= this->count() );
    this->value_init( this->count(), x );
    for( size_t j = this->count(); j > i; --j )
    {
        this->value_swap( j, this, j - 1 );
    }
    this->set_count( this->count() + 1 );

    if( !this->leaf() )
    {
        ++i;
        for( size_t j = this->count(); j > i; --j )
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
template< typename K, typename D >
void Internal< K, D >::remove_value( size_t i )
{
    if( !this->leaf() )
    {
        assert( child( i + 1 )->count() == 0 );
        for( size_t j = i + 1; j < this->count(); ++j )
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
// Rebalances a node with its right sibling.
//
template< typename K, typename D >
void Internal< K, D >::rebalance_right_to_left( Internal *src, size_t to_move )
{
    assert( this->parent() == src->parent() );
    assert( this->position() + 1 == src->position() );
    assert( src->count() >= this->count() );
    assert( to_move >= 1 );
    assert( to_move <= src->count() );

    // Make room in the left node for the new values.
    for( size_t i = 0; i < to_move; ++i )
    {
        this->value_init( i + this->count() );
    }

    // Move the delimiting value to the left node and the new delimiting value
    // from the right node.
    this->value_swap( this->count(), this->parent(), this->position() );
    this->parent()->value_swap( this->position(), src, to_move - 1 );

    // Move the values from the right to the left node.
    for( size_t i = 1; i < to_move; ++i )
    {
        this->value_swap( this->count() + i, src, i - 1 );
    }

    // Shift the values in the right node to their correct position.
    for( size_t i = to_move; i < src->count(); ++i )
    {
        src->value_swap( i - to_move, src, i );
    }

    for( size_t i = 1; i <= to_move; ++i )
    {
        src->value_destroy( src->count() - i );
    }

    if( !this->leaf() )
    {
        // Move the child pointers from the right to the left node.
        for( size_t i = 0; i < to_move; ++i )
        {
            set_child( 1 + this->count() + i, src->child( i ) );
        }

        for( size_t i = 0; i <= src->count() - to_move; ++i )
        {
            assert( i + to_move <= src->max_count() );
            src->set_child( i, src->child( i + to_move ) );
            *src->mutable_child( i + to_move ) = nullptr;
        }
    }

    // Fixup the counts on the src and dest nodes.
    this->set_count( this->count() + to_move );
    src->set_count( src->count() - to_move );
}

//
// Rebalances a node with its right sibling.
//
template< typename K, typename D >
void Internal< K, D >::rebalance_left_to_right(Internal *dest, size_t to_move )
{
    assert( this->parent() == dest->parent() );
    assert( this->position() + 1 == dest->position() );
    assert( this->count() >= dest->count() );
    assert( to_move >= 1 );
    assert( to_move <= this->count() );

    // Make room in the right node for the new values.
    for( size_t i = 0; i < to_move; ++i )
    {
        dest->value_init( i + dest->count() );
    }

    for( int i = dest->count() - 1; i >= 0; --i )
    {
        dest->value_swap( i, dest, i + to_move );
    }

    // Move the delimiting value to the right node and the new delimiting value
    // from the left node.
    dest->value_swap( to_move - 1, this->parent(), this->position() );
    this->parent()->value_swap( this->position(), this, this->count() - to_move );
    this->value_destroy( this->count() - to_move );

    // Move the values from the left to the right node.
    for( size_t i = 1; i < to_move; ++i )
    {
        this->value_swap( this->count() - to_move + i, dest, i - 1 );
        this->value_destroy( this->count() - to_move + i );
    }

    if( !this->leaf() )
    {
        // Move the child pointers from the left to the right node.
        for( int i = dest->count(); i >= 0; --i )
        {
            dest->set_child( i + to_move, dest->child( i ) );
            *dest->mutable_child( i ) = nullptr;
        }
        for( size_t i = 1; i <= to_move; ++i )
        {
            dest->set_child( i - 1, this->child( this->count() - to_move + i ) );
            *this->mutable_child( this->count() - to_move + i ) = nullptr;
        }
    }

    // Fixup the counts on the src and dest nodes.
    this->set_count( this->count() - to_move );
    dest->set_count( dest->count() + to_move );
}

//
// Splits a node, moving a portion of the node's values to its right sibling.
//
template< typename K, typename D >
void Internal< K, D >::split( Internal *dest, size_t insert_position )
{
    assert( dest->count() == 0 );

    // We bias the split based on the position being inserted. If we're
    // inserting at the beginning of the left node then bias the split to put
    // more values on the right node. If we're inserting at the end of the
    // right node then bias the split to put more values on the left node.
    if( insert_position == 0 )
    {
        dest->set_count( this->count() - 1 );
    }
    else if( insert_position == this->max_count() )
    {
        dest->set_count( 0 );
    }
    else
    {
        dest->set_count( this->count() / 2 );
    }
    this->set_count( this->count() - dest->count() );
    assert( this->count() >= 1 );

    // Move values from the left sibling to the right sibling.
    for( size_t i = 0; i < dest->count(); ++i )
    {
        dest->value_init( i );
        this->value_swap( this->count() + i, dest, i );
        this->value_destroy( this->count() + i );
    }

    // The split key is the largest value in the left sibling.
    this->set_count( this->count() - 1 );
    this->parent()->insert_value( this->position(), value_type() );
    this->value_swap( this->count(), this->parent(), this->position() );
    this->value_destroy( this->count() );
    this->parent()->set_child( this->position() + 1, dest );

    if( !this->leaf() )
    {
        for( size_t i = 0; i <= dest->count(); ++i )
        {
            assert( this->child( this->count() + i + 1 ) );
            dest->set_child( i, this->child( this->count() + i + 1 ) );
            *this->mutable_child( this->count() + i + 1 ) = nullptr;
        }
    }
}

//
// Merges a node with its right sibling, moving all of the values and the
// delimiting key in the parent node onto itself.
//
template< typename K, typename D >
void Internal< K, D >::merge( Internal *src )
{
    assert( this->parent() == src->parent() );
    assert( this->position() + 1 == src->position() );

    // Move the delimiting value to the left node.
    this->value_init( this->count() );
    this->value_swap( this->count(), this->parent(), this->position() );

    // Move the values from the right to the left node.
    for( int i = 0; i < src->count(); ++i )
    {
        this->value_init( 1 + this->count() + i );
        this->value_swap( 1 + this->count() + i, src, i );
        src->value_destroy( i );
    }

    if( !this->leaf() )
    {
        // Move the child pointers from the right to the left node.
        for( int i = 0; i <= src->count(); ++i )
        {
            set_child( 1 + this->count() + i, src->child( i ) );
            *src->mutable_child( i ) = nullptr;
        }
    }

    // Fixup the counts on the src and dest nodes.
    this->set_count( 1 + this->count() + src->count() );
    src->set_count( 0 );

    // Remove the value on the parent node.
    this->parent()->remove_value( this->position() );
}

//
// Swap the contents of "this" and "src".
//
template< typename K, typename D >
void Internal< K, D >::swap( Internal *x )
{
    assert( this->leaf() == x->leaf() );

    // Swap the values.
    for( size_t i = this->count(); i < x->count(); ++i )
    {
        this->value_init( i );
    }

    for( size_t i = x->count(); i < this->count(); ++i )
    {
        x->value_init( i );
    }

    const size_t n = std::max( this->count(), x->count() );
    for( size_t i = 0; i < n; ++i )
    {
        this->value_swap( i, x, i );
    }

    for( size_t i = this->count(); i < x->count(); ++i )
    {
        x->value_destroy( i );
    }

    for( size_t i = x->count(); i < this->count(); ++i )
    {
        this->value_destroy( i );
    }


    if( !this->leaf() )
    {
        // Swap the child pointers.
        for( size_t i = 0; i <= n; ++i )
        {
            std::swap( *this->mutable_child( i ), *x->mutable_child( i ) );
        }

        for( size_t i = 0; i <= this->count(); ++i )
        {
            x->child( i )->m_parent = x;
        }

        for( size_t i = 0; i <= x->count(); ++i )
        {
            this->child(  i )->m_parent = this;
        }
    }

    // Swap the counts.
    std::swap( this->m_count, x->m_count );
}

//
//
//
template< typename K, typename D >
void Internal< K, D >::destroy()
{
    for( int i = 0; i < this->count(); ++i)
    {
        this->value_destroy( i );
    }
}

//
//
//
template< typename K, typename D >
Internal< K, D >* Internal< K, D >::get_internal()
{
    return this;
}

//
//
//
template< typename K, typename D >
const Internal< K, D >* Internal< K, D >::get_internal() const
{
    return this;
}

}

#endif

