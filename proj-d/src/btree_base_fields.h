#ifndef ROMZ_BTREE_BASE_FIELDS_H
#define ROMZ_BTREE_BASE_FIELDS_H


#include <cassert>

namespace btree
{

template< typename Params >
class btree_node;

template< typename Params >
class btree_base_fields
{
public:
    typedef typename Params::node_count_type field_type;

public:
    bool leaf() const;

    field_type position() const;
    void set_position( field_type v );

    field_type count() const;
    void set_count( field_type v );
    field_type max_count() const;

    btree_node< Params >* parent() const;

    bool is_root() const;
    void make_root();

    void init( btree_node< Params > *parent, int max_count );

protected:
    // A boolean indicating whether the node is a leaf or not.
    bool m_leaf;

    // The position of the node in the node's parent.
    field_type m_position;

    // The maximum number of values the node can hold.
    field_type m_max_count;

    // The count of the number of values in the node.
    field_type m_count;

    // A pointer to the node's parent.
    btree_node< Params > *m_parent;
};

//
//
// Getter/setter for whether this is a leaf node or not. This value doesn't
// change after the node is created.
//
//
template< typename P >
bool btree_base_fields< P >::leaf() const
{
    return m_leaf;
}

//
// Getter for the position of this node in its parent.
//
template< typename P >
typename btree_base_fields< P >::field_type
btree_base_fields< P >::position() const
{
    return m_position;
}

//
//
//
template< typename P >
void btree_base_fields< P >::set_position( field_type v )
{
    m_position = v;
}

//
// Getter/setter for the number of values stored in this node.
//
template< typename P >
typename btree_base_fields< P >::field_type
btree_base_fields< P >::count() const
{
    return m_count;
}

//
//
//
template< typename P >
void btree_base_fields< P >::set_count( field_type v )
{
    m_count = v;
}

//
//
//
template< typename P >
typename btree_base_fields< P >::field_type
btree_base_fields< P >::max_count() const
{
    return m_max_count;
}

//
// Getter for the parent of this node.
//
template< typename P >
btree_node< P >* btree_base_fields< P >::parent() const
{
    return m_parent;
}

//
// Getter for whether the node is the root of the tree. The parent of the
// root of the tree is the leftmost node in the tree which is guaranteed to
// be a leaf.
//
template< typename P >
bool btree_base_fields< P >::is_root() const
{
    assert( parent() );
    return parent()->leaf();
}

//
//
//
template< typename P >
void btree_base_fields< P >::make_root()
{
    assert( parent()->is_root() );
    m_parent = m_parent->parent();
}

//
//
//
template< typename P >
void btree_base_fields< P >::init( btree_node< P > *parent, int max_count )
{
    m_leaf = true;
    m_position = 0;
    m_max_count = max_count;
    m_count = 0;
    m_parent = parent;
}

}

#endif
