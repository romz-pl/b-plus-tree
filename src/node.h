#ifndef BPLUSTREE_NODE_H
#define BPLUSTREE_NODE_H

#include <string>
#include "definitions.h"

// Key used where only the entry's pointer has meaning.
const KeyType DUMMY_KEY{-1};

// Abstract class.
class Node
{
public:
    explicit Node(size_t aOrder);
    explicit Node(size_t aOrder, Node* aParent);
    virtual ~Node();
    size_t order() const;
    Node* parent() const;
    void setParent(Node* aParent);
    bool isRoot() const;
    virtual bool isLeaf() const = 0;
    virtual size_t size() const = 0;
    virtual size_t minSize() const = 0;
    virtual size_t maxSize() const = 0;
    virtual std::string toString(bool aVerbose = false) const = 0;
private:
    const size_t fOrder;
    Node* fParent;
};

#endif

