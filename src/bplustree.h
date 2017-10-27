#ifndef BPLUSTREE_BPLUSTREE_H
#define BPLUSTREE_BPLUSTREE_H

#include <tuple>
#include <vector>
#include "definitions.h"
#include "printer.h"

class InternalNode;
class LeafNode;
class Node;


/// Main class providing the API for the Interactive B+ Tree.
class BPlusTree
{
public:
    explicit BPlusTree( size_t order = DEFAULT_ORDER );
    
    /// The type used in the API for inserting a new key-value pair
    /// into the tree.  The third item is the type of the Node into
    /// which the key will be inserted.
    using EntryType = std::tuple< KeyType, ValueType, LeafNode* >;
    

    bool isEmpty() const;  
    void insert( KeyType key, ValueType value );
    void remove( KeyType key );
    

    void print( bool verbose = false );
    void printLeaves( bool verbose = false );
    void printValue( KeyType key, bool verbose = false );
    void printPathTo( KeyType key, bool verbose = false );
    void printRange( KeyType start, KeyType end );


    void destroyTree();
    void readInputFromFile( std::string fileName );

private:
    void startNewTree( KeyType key, ValueType value );
    void insertIntoLeaf( KeyType key, ValueType value );
    void insertIntoParent( Node* oldNode, KeyType key, Node* newNode );
    template < typename T > T* split( T* aNode );
    void removeFromLeaf( KeyType key );
    template < typename N > void coalesceOrRedistribute( N* node );
    template < typename N > void coalesce( N* neighborNode, N* node, InternalNode* parent, int index );
    template < typename N > void redistribute( N* neighborNode, N* node, InternalNode* parent, int index );
    void adjustRoot();
    LeafNode* findLeafNode( KeyType key, bool printing = false, bool verbose = false );
    void printValue( KeyType key, bool printPath, bool verbose );
    std::vector< EntryType > range( KeyType start, KeyType end );

private:
    const size_t m_order;
    Node* m_root;
    Printer m_printer;
};

#endif

