#include "node.h"
#include <cassert>

// Declaration and initialization of static member
size_t Node::m_order = 4;

//
//
//
Node::Node( Internal* parent )
    : m_parent{ parent }
{

}

//
//
//
size_t Node::GetOrder()
{
    return m_order;
}

//
//
//
void Node::SetOrder( size_t order )
{
    m_order = order;
}

//
//
//
Internal *Node::GetParent() const
{
    return m_parent;
}

//
//
//
void Node::SetParent( Internal *parent )
{
    m_parent = parent;
}

//
//
//
bool Node::IsRoot() const
{
    return !m_parent;
}

//
//
//
Internal* Node::GetInternal()
{
    assert( 0 );
    return nullptr;
}

//
//
//
Leaf* Node::GetLeaf()
{
    assert( 0 );
    return nullptr;
}
