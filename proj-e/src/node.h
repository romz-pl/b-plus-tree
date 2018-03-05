#ifndef ROMZ_BTREE_NODE_H
#define ROMZ_BTREE_NODE_H


#include <cassert>
#include <cstddef>

namespace btree
{

template< typename Key, typename Data >
class Leaf;

template< typename Key, typename Data >
class Internal;


template< typename Key, typename Data >
class Node
{
public:
    typedef std::pair< const Key, Data >   value_type;
    typedef std::pair< Key, Data >         mutable_value_type;
    typedef Key                            key_type;
    typedef value_type&                    reference;
    typedef const value_type&              const_reference;
    typedef value_type*                    pointer;
    typedef const value_type*              const_pointer;
    typedef size_t                         size_type;
    typedef ptrdiff_t                      difference_type;

public:
    Node( );
    virtual ~Node() = default;

    void init( Internal< Key, Data > *parent, size_t max_count );

    bool leaf() const;

    size_t position() const;
    void set_position( size_t v );

    size_t count() const;
    void set_count( size_t v );
    size_t max_count() const;

    Internal< Key, Data >* parent() const;

    bool is_root() const;
    void make_root();

    virtual Leaf< Key, Data >* get_leaf();
    virtual const Leaf< Key, Data >* get_leaf() const;

    virtual Internal< Key, Data >* get_internal();
    virtual const Internal< Key, Data >* get_internal() const;


// protected:
    // A boolean indicating whether the node is a leaf or not.
    bool m_leaf;

    // The position of the node in the node's parent.
    size_t m_position;

    // The maximum number of values the node can hold.
    size_t m_max_count;

    // The count of the number of values in the node.
    size_t m_count;

    // A pointer to the node's parent.
    Internal< Key, Data > *m_parent;
};

//
//
//
template< typename K, typename D >
Node< K, D >::Node( )
    : m_leaf( true )
    , m_position( 0 )
    , m_max_count( 0 )
    , m_count( 0 )
    , m_parent( nullptr )
{
}


//
//
//
template< typename K, typename D >
void Node< K, D >::init( Internal< K, D > *parent, size_t max_count )
{
    this->m_leaf = true;
    this->m_position = 0;
    this->m_max_count = max_count;
    this->m_count = 0;
    this->m_parent = parent;
}

//
//
// Getter/setter for whether this is a leaf node or not. This value doesn't
// change after the node is created.
//
//
template< typename K, typename D >
bool Node< K, D >::leaf() const
{
    return m_leaf;
}

//
// Getter for the position of this node in its parent.
//
template< typename K, typename D >
size_t Node< K, D >::position() const
{
    return m_position;
}

//
//
//
template< typename K, typename D >
void Node< K, D >::set_position( size_t v )
{
    m_position = v;
}

//
// Getter/setter for the number of values stored in this node.
//
template< typename K, typename D >
size_t Node< K, D >::count() const
{
    return m_count;
}

//
//
//
template< typename K, typename D >
void Node< K, D >::set_count( size_t v )
{
    m_count = v;
}

//
//
//
template< typename K, typename D >
size_t Node< K, D >::max_count() const
{
    return m_max_count;
}

//
// Getter for the parent of this node.
//
template< typename K, typename D >
Internal< K, D >* Node< K, D >::parent() const
{
    return m_parent;
}

//
// Getter for whether the node is the root of the tree. The parent of the
// root of the tree is the leftmost node in the tree which is guaranteed to
// be a leaf.
//
template< typename K, typename D >
bool Node< K, D >::is_root() const
{
    assert( parent() );
    return parent()->leaf();
}

//
//
//
template< typename K, typename D >
void Node< K, D >::make_root()
{
    assert( parent()->is_root() );
    m_parent = m_parent->parent();
}

//
//
//
template< typename K, typename D >
Leaf< K, D >* Node< K, D >::get_leaf()
{
    assert( 0 );
    return nullptr;
}

//
//
//
template< typename K, typename D >
const Leaf< K, D >* Node< K, D >::get_leaf() const
{
    assert( 0 );
    return nullptr;
}

//
//
//
template< typename K, typename D >
Internal< K, D >* Node< K, D >::get_internal()
{
    assert( 0 );
    return nullptr;
}

//
//
//
template< typename K, typename D >
const Internal< K, D >* Node< K, D >::get_internal() const
{
    assert( 0 );
    return nullptr;
}

}

#endif
