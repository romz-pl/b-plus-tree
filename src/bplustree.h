#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

#include <tuple>
#include <vector>
#include "definitions.h"
#include "key.h"
#include "value.h"

class InternalNode;
class LeafNode;
class Node;



/// Main class providing the API for the Interactive B+ Tree.
class BPlusTree
{
public:
    explicit BPlusTree( size_t order = DEFAULT_ORDER );
    
    bool isEmpty() const;  
    Value get( Key key );
    void insert( Key key, Value value );
    void remove( Key key );
    void destroyTree();
    size_t count() const;

private:
    void startNewTree( Key key, Value value );
    void insertIntoLeaf( Key key, Value value );
    void insertIntoParent( Node* oldNode, Key key, Node* newNode );
    template < typename T > T* split( T* aNode );
    void removeFromLeaf( Key key );
    template < typename N > void coalesceOrRedistribute( N* node );
    template < typename N > void coalesce( N* neighborNode, N* node, InternalNode* parent, size_t index );
    template < typename N > void redistribute( N* neighborNode, N* node, InternalNode* parent, size_t index );
    void adjustRoot();
    LeafNode* findLeafNode( Key key, bool printing = false, bool verbose = false );

private:
    const size_t m_order;
    Node* m_root;

    size_t m_count;
};

#endif

