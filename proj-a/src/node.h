#ifndef BPLUSTREE_NODE_H
#define BPLUSTREE_NODE_H

#include <string>
#include "key.h"

class InternalNode;
class LeafNode;

// Abstract class.
class Node
{
public:
    explicit Node( size_t order, InternalNode* parent );
    virtual ~Node() = default;

    size_t order() const;
    InternalNode* getParent() const;
    void setParent( InternalNode* parent );
    bool isRoot() const;

    virtual InternalNode* getInternalNode();
    virtual LeafNode* getLeafNode();

    virtual bool isLeaf() const = 0;
    virtual size_t size() const = 0;
    virtual size_t minSize() const = 0;
    virtual size_t maxSize() const = 0;
    virtual std::string toString( bool aVerbose = false ) const = 0;
private:
    const size_t m_order;

    InternalNode* m_parent;
};

#endif

