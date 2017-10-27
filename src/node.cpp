#include "node.h"

Node::Node(size_t aOrder) : fOrder{aOrder}, fParent{nullptr} {}

Node::Node(size_t aOrder, Node* aParent) : fOrder{aOrder}, fParent{aParent} {}

Node::~Node() {}

size_t Node::order() const
{
    return fOrder;
}

Node* Node::parent() const
{
    return fParent;
}

void Node::setParent(Node* aParent)
{
    fParent = aParent;
}

bool Node::isRoot() const
{
    return !fParent;
}
