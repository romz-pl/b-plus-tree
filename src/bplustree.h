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
    Value get( const Key& key );
    void insert( const Key& key, const Value& value );
    void remove( const Key& key );
    void destroyTree();
    size_t count() const;

private:
    void startNewTree( const Key& key, const Value& value );
    void insertIntoLeaf( const Key& key, const Value& value );
    void insertIntoParent( Node* oldNode, const Key& key, Node* newNode );

    void removeFromLeaf( const Key& key );
    template < typename N > void coalesceOrRedistribute( N* node );
    template < typename N > void coalesce( N* neighborNode, N* node, InternalNode* parent, size_t index );
    template < typename N > void redistribute( N* neighborNode, N* node, size_t index );

    void adjustRoot();
    LeafNode* findLeafNode( const Key& key) ;

private:
    const size_t m_order;
    Node* m_root;

    size_t m_count;
};

#endif

