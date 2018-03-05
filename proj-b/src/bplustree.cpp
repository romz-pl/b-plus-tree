#include "bplustree.h"
#include <cassert>
#include <stdexcept>

//
// Accepts an order for the B+ Tree.
//
BPlusTree::BPlusTree( size_t order)
    : m_root{ nullptr }
{
    if( order < 2 )
    {
        throw std::runtime_error( "Order must be at least 2." );
    }

    Node::SetOrder( order );
}

//
//
//
BPlusTree::~BPlusTree()
{
    if( !m_root )
    {
        return;
    }

    delete m_root;
    m_root = nullptr;
}

//
// Returns true if this B+ tree has no keys or values.
//
bool BPlusTree::IsEmpty() const
{
    return !m_root;
}

//
//
//
Value BPlusTree::Get( const Key& key )
{
    const Value* v = Find( key );
    if( !v )
    {
        throw std::runtime_error( "BPlusTree::Get: Key not found." );
    }
    return *v;
}

//
// Insert a key-value pair into this B+ tree.
//
void BPlusTree::Insert( const Key& key, const Value& value )
{
    if( !m_root )
    {
        m_root = new Leaf( nullptr );
        m_root->GetLeaf()->Append( key, value );
        return;
    }

    Node* node = m_root;
    if( !node->IsLeaf() )
    {
        node = node->GetInternal()->GetChild( key );
    }

    assert( node->IsLeaf() );
    Leaf* leaf = node->GetLeaf();
    if( !leaf->IsFull() )
    {
        leaf->Insert( key, value );
    }
    else
    {
        std::pair< Leaf*, Key > ret = leaf->SplitInsert( key, value );
        InsertInParent( leaf, ret.second, ret.first );
    }
}

//
//
//
void BPlusTree::InsertInParent( Leaf* oldL, const Key& key, Leaf* newL )
{
    if( newL->IsRoot() )
    {
        Internal* w = new Internal( nullptr );
        w->Init( oldL, key, newL );
        newL->SetParent( w );
        oldL->SetParent( w );
        return;
    }

    Internal* parent = oldL->GetParent();
    if( !parent->IsFull() )
    {
        parent->Insert( oldL, key, newL );
    }
    else
    {
        // parent->SplitInsert( key, newL );
    }
}

//
//
//
void BPlusTree::Remove( const Key& /*key*/ )
{
    assert( 0 );
}

//
//
//
size_t BPlusTree::GetSize() const
{
    assert( 0 );
    return 0;
}

//
//
//
Value *BPlusTree::Find( const Key& key )
{
    Node* node = m_root;

    if( !node->IsLeaf() )
    {
        node = node->GetInternal()->GetChild( key );
    }

    assert( node->IsLeaf() );

    return node->GetLeaf()->Find( key );
}

