#include "internal.h"
#include <cassert>
#include <algorithm>
#include <cmath>


//
//
//
Internal::Internal( Internal* parent )
    : Node( parent )
{

    const size_t order = Node::GetOrder();

    m_key.reserve( order - 1);
    m_child.reserve( order );

}

//
//
//
Internal* Internal::GetInternal()
{
    return this;
}

//
//
//
bool Internal::IsLeaf() const
{
    return false;
}

//
//
//
bool Internal::IsFull() const
{
    const size_t order = Node::GetOrder();

    return ( m_key.size() >= order );
}

//
//
//
Node* Internal::GetChild( const Key& key )
{
    assert( std::is_sorted( m_key.begin(), m_key.end() ) );
    auto it = std::lower_bound( m_key.begin(), m_key.end(), key );

    if( it == m_key.end() )
    {
        return m_child.back();
    }

    const auto dist = std::distance( it, m_key.begin() );
    assert( dist >= 0 );

    if( *it == key )
    {
        return m_child[ dist + 1 ];
    }

    assert( *it < key );
    return m_child[ dist ];
}

//
//
//
 void Internal::Init( Node *left, const Key& key, Node *right )
 {
     assert( m_key.empty() );
     assert( m_child.empty() );

     m_key.push_back( key );
     m_child.push_back( left );
     m_child.push_back( right );
 }

 //
 //
 //
 void Internal::Insert( Node* after, const Key& key, Node* child )
 {
     auto it = std::find( m_child.begin(), m_child.end(), after );
     assert( it != m_child.end() );
     it++;
     m_child.insert( it, child );

     const auto dist = std::distance( m_child.begin(), it );
     m_key.insert( m_key.begin() + dist, key );
 }

 //
 //
 //
 void Internal::SplitInsert( Node* after, const Key& key, Node* child )
 {
    Insert( after, key, child );

    const size_t mid = ceil( Node::GetOrder() / 2 );

    Internal* w = new Internal( nullptr );

    for( size_t i = mid + 1; i < m_key.size(); i++ )
    {
        // w->Append()
    }
 }
