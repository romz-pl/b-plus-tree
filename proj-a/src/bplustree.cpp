#include <iostream>
#include <fstream>
#include <string>
#include "bplustree.h"
#include "internalnode.h"
#include "leafnode.h"
#include "node.h"
#include <cassert>
#include <stdexcept>

//
// Sole constructor.
// Accepts an order for the B+ Tree.
//
BPlusTree::BPlusTree( size_t order)
    : m_order{ order }
    , m_root{ nullptr }
    , m_count( 0 )
{

}

//
//
//
BPlusTree::~BPlusTree()
{
    destroyTree();
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
Value BPlusTree::get( const Key &key )
{
    LeafNode* leafNode = findLeafNode( key );
    if( !leafNode )
    {
        throw std::runtime_error( "Key not found (LeafNode)" );
    }

    Record * rec = leafNode->lookup( key );
    if( !rec )
    {
        throw std::runtime_error( "Key not found (Record)" );
    }
    return rec->value();
}

//
//
//
size_t BPlusTree::count() const
{
    return m_count;
}

//
// Insert a key-value pair into this B+ tree.
//
void BPlusTree::insert( const Key& key, const Value& value )
{
    if( isEmpty() )
    {
        startNewTree( key, value );
    }
    else
    {
        insertIntoLeaf( key, value );
    }
    m_count++;
}

//
//
//
void BPlusTree::startNewTree( const Key& key, const Value& value )
{
    assert( !m_root );
    m_root = new LeafNode( m_order, nullptr );
    m_root->getLeafNode()->createAndInsertRecord( key, value );
}

//
//
//
void BPlusTree::insertIntoLeaf( const Key& key, const Value& value )
{
    LeafNode* leafNode = findLeafNode( key );
    if( !leafNode )
    {
        throw std::runtime_error( "Key not found in any leaf node" );
    }

    const size_t newSize = leafNode->createAndInsertRecord( key, value );
    if( newSize > leafNode->maxSize() )
    {
        LeafNode* newLeaf = leafNode->split( m_order );
        newLeaf->setNext( leafNode->next() );
        leafNode->setNext( newLeaf );
        const Key newKey = newLeaf->firstKey();
        insertIntoParent( leafNode, newKey, newLeaf );
    }
}

//
//
//
void BPlusTree::insertIntoParent( Node *oldNode, const Key& key, Node *newNode )
{
    InternalNode* parent = oldNode->getParent();
    if( !parent )
    {
        m_root = new InternalNode( m_order, nullptr );
        parent = m_root->getInternalNode();
        oldNode->setParent( parent );
        newNode->setParent( parent );
        parent->populateNewRoot( oldNode, key, newNode );
    }
    else
    {
        const size_t newSize = parent->insertNodeAfter( oldNode, key, newNode );
        if( newSize > parent->maxSize() )
        {
            InternalNode* newNode = parent->split( m_order );
            const Key newKey = newNode->replaceAndReturnFirstKey();
            insertIntoParent( parent, newKey, newNode );
        }
    }
}



//
// Remove a key and its value from this B+ tree.
//
void BPlusTree::remove( const Key& key )
{
    if( isEmpty() )
    {
        return;
    }

    removeFromLeaf( key );
}

//
//
//
void BPlusTree::removeFromLeaf( const Key& key )
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

    const size_t newSize = leafNode->removeAndDeleteRecord( key );
    if( newSize < leafNode->minSize() )
    {
        coalesceOrRedistribute( leafNode );
    }
    m_count--;
}

//
//
//
void BPlusTree::coalesceOrRedistribute( LeafNode* node )
{
    if( node->isRoot() )
    {
        adjustRoot();
        return;
    }

    InternalNode* parent = node->getParent();
    const size_t indexOfNodeInParent = parent->nodeIndex( node );
    const size_t neighborIndex = ( indexOfNodeInParent == 0 ) ? 1 : indexOfNodeInParent - 1;
    LeafNode* neighborNode = parent->neighbor( neighborIndex )->getLeafNode();
    if( node->size() + neighborNode->size() <= neighborNode->maxSize() )
    {
        coalesce( neighborNode, node, parent, indexOfNodeInParent );
    }
    else
    {
        neighborNode->redistribute( node, indexOfNodeInParent );
    }
}

//
//
//
void BPlusTree::coalesceOrRedistribute( InternalNode* node )
{
    if( node->isRoot() )
    {
        adjustRoot();
        return;
    }

    InternalNode* parent = node->getParent();
    const size_t indexOfNodeInParent = parent->nodeIndex( node );
    const size_t neighborIndex = ( indexOfNodeInParent == 0 ) ? 1 : indexOfNodeInParent - 1;
    InternalNode* neighborNode = parent->neighbor( neighborIndex )->getInternalNode();
    if( node->size() + neighborNode->size() <= neighborNode->maxSize() )
    {
        coalesce( neighborNode, node, parent, indexOfNodeInParent );
    }
    else
    {
        neighborNode->redistribute( node, indexOfNodeInParent );
    }
}

//
//
//
void BPlusTree::coalesce( LeafNode* neighborNode, LeafNode* node, InternalNode* parent, size_t index )
{
    if( index == 0 )
    {
        std::swap( node, neighborNode );
        index = 1;
    }
    node->moveAllTo( neighborNode );
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
void BPlusTree::coalesce( InternalNode* neighborNode, InternalNode* node, InternalNode* parent, size_t index )
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
void BPlusTree::adjustRoot()
{
    if( !m_root->isLeaf() && m_root->size() == 1 )
    {
        InternalNode* discardedNode = m_root->getInternalNode();
        m_root = m_root->getInternalNode()->removeAndReturnOnlyChild();
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
//
//
LeafNode* BPlusTree::findLeafNode( const Key& key )
{
    if( isEmpty() )
    {
        return nullptr;
    }

    Node* node = m_root;
    while( !node->isLeaf() )
    {
        node = node->getInternalNode()->lookup( key );
    }
    return node->getLeafNode();
}

//
// Remove all elements from the B+ tree. You can then build
// it up again by inserting new elements into it.
//
void BPlusTree::destroyTree()
{
    if( !m_root )
    {
        return;
    }

    delete m_root;
    m_root = nullptr;
    m_count = 0;
}

