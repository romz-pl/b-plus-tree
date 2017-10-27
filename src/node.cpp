#include "node.h"

//
//
//
Node::Node( size_t order )
    : m_order{ order }
    , m_parent{ nullptr }
{

}

//
//
//
Node::Node( size_t order, Node* parent )
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
Node* Node::parent() const
{
    return m_parent;
}

//
//
//
void Node::setParent( Node* parent )
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
