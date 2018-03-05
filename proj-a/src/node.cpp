#include "node.h"
#include <cassert>

//
//
//
Node::Node( size_t order, InternalNode* parent )
    : m_order{ order }
    , m_parent{ parent }
{

}

//
//
//
size_t Node::order() const
{
    return m_order;
}

//
//
//
InternalNode* Node::getParent() const
{
    return m_parent;
}

//
//
//
void Node::setParent( InternalNode *parent )
{
    m_parent = parent;
}

//
//
//
bool Node::isRoot() const
{
    return !m_parent;
}

//
//
//
InternalNode* Node::getInternalNode()
{
    assert( 0 );
    return nullptr;
}

//
//
//
LeafNode* Node::getLeafNode()
{
    assert( 0 );
    return nullptr;
}
