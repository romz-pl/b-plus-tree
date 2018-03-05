#ifndef ROMZ_BTREE_BTREE_NODE_H
#define ROMZ_BTREE_BTREE_NODE_H

#include <type_traits>
#include <cstring>

#include "btree_swap_helper.h"
#include "btree_compare_keys.h"
#include "btree_linear_search_plain_compare.h"
#include "btree_linear_search_compare_to.h"
#include "btree_binary_search_plain_compare.h"
#include "btree_binary_search_compare_to.h"

#include "btree_base_fields.h"
#include "btree_leaf_fields.h"
#include "btree_internal_fields.h"
#include "btree_root_fields.h"

namespace btree
{

//
// A node in the btree holding. The same node type is used for both internal
// and leaf nodes in the btree, though the nodes are allocated in such a way
// that the children array is only valid in internal nodes.
//
template< typename Params >
class btree_node : public btree_root_fields< Params >
{
public:
    typedef Params                              params_type;
    typedef btree_node< Params >                self_type;
    typedef typename Params::key_type           key_type;
    typedef typename Params::data_type          data_type;
    typedef typename Params::value_type         value_type;
    typedef typename Params::mutable_value_type mutable_value_type;
    typedef typename Params::pointer            pointer;
    typedef typename Params::const_pointer      const_pointer;
    typedef typename Params::reference          reference;
    typedef typename Params::const_reference    const_reference;
    typedef typename Params::key_compare        key_compare;
    typedef typename Params::size_type          size_type;
    typedef typename Params::difference_type    difference_type;

    // Typedefs for the various types of node searches.
    typedef btree_linear_search_plain_compare< key_type, self_type, key_compare > linear_search_plain_compare_type;
    typedef btree_linear_search_compare_to< key_type, self_type, key_compare >    linear_search_compare_to_type;
    typedef btree_binary_search_plain_compare< key_type, self_type, key_compare > binary_search_plain_compare_type;
    typedef btree_binary_search_compare_to< key_type, self_type, key_compare >    binary_search_compare_to_type;

    // If we have a valid key-compare-to type, use linear_search_compare_to,
    // otherwise use linear_search_plain_compare.
    typedef typename std::conditional< Params::is_key_compare_to::value,
                          linear_search_compare_to_type,
                          linear_search_plain_compare_type >::type linear_search_type;

    // If we have a valid key-compare-to type, use binary_search_compare_to,
    // otherwise use binary_search_plain_compare.
    typedef typename std::conditional< Params::is_key_compare_to::value,
                          binary_search_compare_to_type,
                          binary_search_plain_compare_type >::type binary_search_type;

    // If the key is an integral or floating point type, use linear search which
    // is faster than binary search for such types. Might be wise to also
    // configure linear search based on node-size.
    typedef typename std::conditional< std::is_integral< key_type >::value || std::is_floating_point< key_type >::value,
                          linear_search_type,
                          binary_search_type>::type search_type;

    enum
    {
        kExactMatch = 1 << 30,
        kMatchMask = kExactMatch - 1,
    };

public:
    btree_node( const btree_node& ) = delete;
    void operator=( const btree_node& ) = delete;

    void set_child( int i, btree_node *c );

    template< typename Compare >
    int lower_bound( const key_type &k, const Compare &comp ) const;

    template< typename Compare >
    int upper_bound( const key_type &k, const Compare &comp ) const;

    template< typename Compare >
    int linear_search_plain_compare( const key_type &k, int s, int e, const Compare &comp ) const;

    template< typename Compare >
    int linear_search_compare_to( const key_type &k, int s, int e, const Compare &comp ) const;

    template < typename Compare >
    int binary_search_plain_compare( const key_type &k, int s, int e, const Compare &comp ) const;

    template< typename CompareTo >
    int binary_search_compare_to( const key_type &k, int s, int e, const CompareTo &comp ) const;

    void rebalance_right_to_left( btree_node *sibling, int to_move );
    void rebalance_left_to_right( btree_node *sibling, int to_move );

    void split( btree_node *sibling, int insert_position );
    void merge( btree_node *sibling );

    void swap( btree_node *src );

    void destroy();
};

//
//
//
template< typename P >
void btree_node< P >::set_child( int i, btree_node *c )
{
    *this->mutable_child( i ) = c;
    c->m_parent = this;
    c->m_position = i;
}

//
// Returns the position of the first value whose key is not less than k.
//
template< typename P >
template< typename Compare >
int btree_node< P >::lower_bound( const key_type &k, const Compare &comp ) const
{
    return search_type::lower_bound( k, *this, comp );
}

//
// Returns the position of the first value whose key is greater than k.
//
template< typename P >
template< typename Compare >
int btree_node< P >::upper_bound( const key_type &k, const Compare &comp ) const
{
    return search_type::upper_bound( k, *this, comp );
}

//
// Returns the position of the first value whose key is not less than k using
// linear search performed using plain compare.
//
template< typename P >
template< typename Compare >
int btree_node< P >::linear_search_plain_compare( const key_type &k, int s, int e, const Compare &comp ) const
{
    while( s < e )
    {
        if( !btree_compare_keys( comp, this->key( s ), k ) )
        {
            break;
        }
        ++s;
    }
    return s;
}

//
// Returns the position of the first value whose key is not less than k using
// linear search performed using compare-to.
//
template< typename P >
template< typename Compare >
int btree_node< P >::linear_search_compare_to( const key_type &k, int s, int e, const Compare &comp ) const
{
    while( s < e )
    {
        int c = comp( this->key( s ), k );
        if( c == 0 )
        {
            return s | kExactMatch;
        }
        else if( c > 0 )
        {
            break;
        }
        ++s;
    }
    return s;
}

//
// Returns the position of the first value whose key is not less than k using
// binary search performed using plain compare.
//
template< typename P >
template < typename Compare >
int btree_node< P >::binary_search_plain_compare( const key_type &k, int s, int e, const Compare &comp ) const
{
    while( s != e )
    {
        int mid = (s + e) / 2;
        if( btree_compare_keys( comp, this->key( mid ), k ) )
        {
            s = mid + 1;
        }
        else
        {
            e = mid;
        }
    }
    return s;
}

//
// Returns the position of the first value whose key is not less than k using
// binary search performed using compare-to.
//
template< typename P >
template< typename CompareTo >
int btree_node< P >::binary_search_compare_to( const key_type &k, int s, int e, const CompareTo &comp ) const
{
    while( s != e )
    {
        int mid = ( s + e ) / 2;
        int c = comp( this->key(mid), k );
        if( c < 0 )
        {
            s = mid + 1;
        }
        else if( c > 0 )
        {
            e = mid;
        }
        else
        {
            // Need to return the first value whose key is not less than k, which
            // requires continuing the binary search. Note that we are guaranteed
            // that the result is an exact match because if "key(mid-1) < k" the
            // call to binary_search_compare_to() will return "mid".
            s = binary_search_compare_to(k, s, mid, comp);
            return s | kExactMatch;
        }
    }
    return s;
}




//
// Rebalances a node with its right sibling.
//
template< typename P >
void btree_node< P >::rebalance_right_to_left( btree_node *src, int to_move )
{
    assert( this->parent() == src->parent() );
    assert( this->position() + 1 == src->position() );
    assert( src->count() >= this->count() );
    assert( to_move >= 1 );
    assert( to_move <= src->count() );

    // Make room in the left node for the new values.
    for( int i = 0; i < to_move; ++i )
    {
        this->value_init( i + this->count() );
    }

    // Move the delimiting value to the left node and the new delimiting value
    // from the right node.
    this->value_swap( this->count(), this->parent(), this->position() );
    this->parent()->value_swap( this->position(), src, to_move - 1 );

    // Move the values from the right to the left node.
    for( int i = 1; i < to_move; ++i )
    {
        this->value_swap( this->count() + i, src, i - 1 );
    }

    // Shift the values in the right node to their correct position.
    for( int i = to_move; i < src->count(); ++i )
    {
        src->value_swap( i - to_move, src, i );
    }

    for( int i = 1; i <= to_move; ++i )
    {
        src->value_destroy( src->count() - i );
    }

    if( !this->leaf() )
    {
        // Move the child pointers from the right to the left node.
        for( int i = 0; i < to_move; ++i )
        {
            set_child( 1 + this->count() + i, src->child( i ) );
        }

        for( int i = 0; i <= src->count() - to_move; ++i )
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
template< typename P >
void btree_node< P >::rebalance_left_to_right( btree_node *dest, int to_move )
{
    assert( this->parent() == dest->parent() );
    assert( this->position() + 1 == dest->position() );
    assert( this->count() >= dest->count() );
    assert( to_move >= 1 );
    assert( to_move <= this->count() );

    // Make room in the right node for the new values.
    for( int i = 0; i < to_move; ++i )
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
    for( int i = 1; i < to_move; ++i )
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
        for( int i = 1; i <= to_move; ++i )
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
template< typename P >
void btree_node< P >::split( btree_node *dest, int insert_position )
{
    assert(dest->count() == 0);

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
    for( int i = 0; i < dest->count(); ++i )
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
        for( int i = 0; i <= dest->count(); ++i )
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
template <typename P>
void btree_node<P>::merge( btree_node *src )
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
template< typename P >
void btree_node< P >::swap( btree_node *x )
{
    assert( this->leaf() == x->leaf() );

    // Swap the values.
    for( int i = this->count(); i < x->count(); ++i )
    {
        this->value_init( i );
    }

    for( int i = x->count(); i < this->count(); ++i )
    {
        x->value_init( i );
    }

    int n = std::max( this->count(), x->count() );
    for( int i = 0; i < n; ++i )
    {
        this->value_swap( i, x, i );
    }

    for( int i = this->count(); i < x->count(); ++i )
    {
        x->value_destroy( i );
    }

    for( int i = x->count(); i < this->count(); ++i )
    {
        this->value_destroy( i );
    }

    if( !this->leaf() )
    {
        // Swap the child pointers.
        for( int i = 0; i <= n; ++i )
        {
            btree_swap_helper( *this->mutable_child( i ), *x->mutable_child( i ) );
        }

        for( int i = 0; i <= this->count(); ++i )
        {
            x->child( i )->m_parent = x;
        }

        for( int i = 0; i <= x->count(); ++i )
        {
            this->child(  i )->m_parent = this;
        }
    }

    // Swap the counts.
    btree_swap_helper( this->m_count, x->m_count );
}

//
//
//
template< typename P >
void btree_node< P >::destroy()
{
    for( int i = 0; i < this->count(); ++i)
    {
        this->value_destroy( i );
    }
}


}

#endif
