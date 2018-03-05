#ifndef BPLUSTREE_NODE_H
#define BPLUSTREE_NODE_H

//
// 1. Base class for Leaf and Internal (non leaf).
//
// 2. Every node has the same order. Hence order is the static member.
//
// 3. For each node, its parent is always of type Internal
//


// #include <string>
#include <cstddef>

class Internal;
class Leaf;

class Node
{
public:
    explicit Node( Internal* parent );
    virtual ~Node() = default;

    static size_t GetOrder();
    static void SetOrder( size_t order );

    Internal* GetParent() const;
    void SetParent( Internal* parent );

    bool IsRoot() const;

    virtual Internal* GetInternal();
    virtual Leaf* GetLeaf();

    virtual bool IsLeaf() const = 0;
private:
    static size_t m_order;

    Internal* m_parent;
};

#endif

