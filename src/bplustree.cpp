#include <iostream>
#include <fstream>
#include <string>
#include "bplustree.h"
#include "exceptions.h"
#include "internalnode.h"
#include "leafnode.h"
#include "node.h"
#include <cassert>

//
// Sole constructor.  Accepts an optional order for the B+ Tree.
// The default order will provide a reasonable demonstration of the
// data structure and its operations.
//
BPlusTree::BPlusTree( size_t order)
    : m_order{ order }
    , m_root{ nullptr }
{

}

//
// Returns true if this B+ tree has no keys or values.
//
bool BPlusTree::isEmpty() const
{
    return !m_root;
}

//
//
//
Value BPlusTree::get( Key key ) const
{
    assert( 0 );
    return Value();
}

//
//
//
size_t BPlusTree::count() const
{
    assert( 0 );
    return 0;
}

//
// Insert a key-value pair into this B+ tree.
//
void BPlusTree::insert( Key key, Value value )
{
    if( isEmpty() )
    {
        startNewTree( key, value );
    }
    else
    {
        insertIntoLeaf( key, value );
    }
}

//
//
//
void BPlusTree::startNewTree( Key key, Value value )
{
    LeafNode* newLeafNode = new LeafNode( m_order );
    newLeafNode->createAndInsertRecord( key, value );
    m_root = newLeafNode;
}

//
//
//
void BPlusTree::insertIntoLeaf( Key key, Value value )
{
    LeafNode* leafNode = findLeafNode( key );
    if( !leafNode )
    {
        throw LeafNotFoundException( key );
    }

    size_t newSize = leafNode->createAndInsertRecord( key, value );
    if( newSize > leafNode->maxSize() )
    {
        LeafNode* newLeaf = split( leafNode );
        newLeaf->setNext( leafNode->next() );
        leafNode->setNext( newLeaf );
        Key newKey = newLeaf->firstKey();
        insertIntoParent( leafNode, newKey, newLeaf );
    }
}

//
//
//
void BPlusTree::insertIntoParent( Node *oldNode, Key key, Node *newNode )
{
    InternalNode* parent = static_cast< InternalNode* >( oldNode->parent() );
    if( parent == nullptr )
    {
        m_root = new InternalNode( m_order );
        parent = static_cast< InternalNode* >( m_root );
        oldNode->setParent( parent );
        newNode->setParent( parent );
        parent->populateNewRoot( oldNode, key, newNode );
    }
    else
    {
        size_t newSize = parent->insertNodeAfter( oldNode, key, newNode );
        if( newSize > parent->maxSize() )
        {
            InternalNode* newNode = split( parent );
            Key newKey = newNode->replaceAndReturnFirstKey();
            insertIntoParent( parent, newKey, newNode );
        }
    }
}

//
//
//
template < typename T >
T* BPlusTree::split( T* node )
{
    T* newNode = new T( m_order, node->parent() );
    node->moveHalfTo( newNode );
    return newNode;
}


//
// Remove a key and its value from this B+ tree.
//
void BPlusTree::remove(Key key)
{
    if( isEmpty() )
    {
        return;
    }
    else
    {
        removeFromLeaf( key );
    }
}

//
//
//
void BPlusTree::removeFromLeaf( Key key )
{
    LeafNode* leafNode = findLeafNode( key );
    if( !leafNode )
    {
        return;
    }
    if( !leafNode->lookup( key ) )
    {
        return;
    }
    size_t newSize = leafNode->removeAndDeleteRecord( key );
    if( newSize < leafNode->minSize() )
    {
        coalesceOrRedistribute( leafNode );
    }
}

//
//
//
template < typename N >
void BPlusTree::coalesceOrRedistribute( N* node )
{
    if( node->isRoot() )
    {
        adjustRoot();
        return;
    }

    auto parent = static_cast< InternalNode* >( node->parent() );
    const size_t indexOfNodeInParent = parent->nodeIndex( node );
    const size_t neighborIndex = ( indexOfNodeInParent == 0 ) ? 1 : indexOfNodeInParent - 1;
    N* neighborNode = static_cast< N* >( parent->neighbor( neighborIndex ) );
    if( node->size() + neighborNode->size() <= neighborNode->maxSize() )
    {
        coalesce( neighborNode, node, parent, indexOfNodeInParent );
    }
    else
    {
        redistribute( neighborNode, node, parent, indexOfNodeInParent );
    }
}

//
//
//
template < typename N >
void BPlusTree::coalesce( N* neighborNode, N* node, InternalNode* parent, size_t index )
{
    if( index == 0 )
    {
        std::swap( node, neighborNode );
        index = 1;
    }
    node->moveAllTo( neighborNode, index );
    parent->remove( index );
    if( parent->size() < parent->minSize() )
    {
        coalesceOrRedistribute( parent );
    }
    delete node;
}

//
//
//
template < typename N >
void BPlusTree::redistribute( N* neighborNode, N* node, InternalNode* /*aParent*/, size_t index )
{
    if( index == 0 )
    {
        neighborNode->moveFirstToEndOf( node );
    }
    else
    {
        neighborNode->moveLastToFrontOf( node, index );
    }
}

//
//
//
void BPlusTree::adjustRoot()
{
    if( !m_root->isLeaf() && m_root->size() == 1 )
    {
        auto discardedNode = static_cast< InternalNode* >( m_root );
        m_root = static_cast< InternalNode* >( m_root )->removeAndReturnOnlyChild();
        m_root->setParent( nullptr );
        delete discardedNode;
    }
    else if( !m_root->size() )
    {
        delete m_root;
        m_root = nullptr;
    }
}


//
// UTILITIES AND PRINTING
//
LeafNode* BPlusTree::findLeafNode( Key key, bool printing, bool verbose )
{
    if( isEmpty() )
    {
        if( printing )
        {
            std::cout << "Not found: empty tree." << std::endl;
        }
        return nullptr;
    }
    auto node = m_root;
    if( printing)
    {
        std::cout << "Root: ";
        if( m_root->isLeaf() )
        {
            std::cout << "\t" << static_cast< LeafNode* >( m_root )->toString( verbose );
        }
        else
        {
            std::cout << "\t" << static_cast< InternalNode* >( m_root )->toString( verbose );
        }
        std::cout << std::endl;
    }
    while( !node->isLeaf() )
    {
        auto internalNode = static_cast< InternalNode* >( node );
        if( printing && node != m_root )
        {
            std::cout << "\tNode: " << internalNode->toString( verbose ) << std::endl;
        }
        node = internalNode->lookup( key );
    }
    return static_cast< LeafNode* >( node );
}




//
// Remove all elements from the B+ tree. You can then build
// it up again by inserting new elements into it.
//
void BPlusTree::destroyTree()
{
    if( m_root->isLeaf() )
    {
        delete static_cast< LeafNode* >( m_root );
    }
    else
    {
        delete static_cast< InternalNode* >( m_root );
    }
    m_root = nullptr;
}




//
//
//
std::vector<BPlusTree::EntryType> BPlusTree::range( Key start, Key end )
{
    auto startLeaf = findLeafNode( start );
    auto endLeaf = findLeafNode( end );
    std::vector< std::tuple< Key, Value, LeafNode* > > entries;
    if( !startLeaf || !endLeaf )
    {
        return entries;
    }

    startLeaf->copyRangeStartingFrom( start, entries );
    startLeaf = startLeaf->next();
    while( startLeaf != endLeaf )
    {
        startLeaf->copyRange( entries );
        startLeaf = startLeaf->next();
    }
    startLeaf->copyRangeUntil( end, entries );
    return entries;
}
