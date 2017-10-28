#ifndef BPLUSTREE_NODE_H
#define BPLUSTREE_NODE_H

#include <string>
#include "definitions.h"
#include "key.h"



// Abstract class.
class Node
{
public:
    explicit Node( size_t order, Node* parent );
    virtual ~Node() = default;

    size_t order() const;
    Node* parent() const;
    void setParent( Node* parent );
    bool isRoot() const;

    virtual bool isLeaf() const = 0;
    virtual size_t size() const = 0;
    virtual size_t minSize() const = 0;
    virtual size_t maxSize() const = 0;
    virtual std::string toString( bool aVerbose = false ) const = 0;
private:
    const size_t m_order;

    Node* m_parent;
};

#endif

